#include "tileprovider.h"
#include <QImage>
#include <QPixmap>
#include <QNetworkReply>
#include "downloadmanager.h"

static const int tileSize = 256;

//Debug
#include <QDebug>

TileProvider::TileProvider(QString url, bool invertX, bool invertY, bool quadKey, QObject *parent) :
	QObject(parent)
{
	init();

	providerUrl_ = url;
	flipX_ = invertX;
	flipY_ = invertY;
	useQuadKey_ = quadKey;
}

void TileProvider::init()
{
	// Set default values
	providerUrl_ = "";
	serverNames_[0] = "";
	nServers_ = 1;
	tileServerCounter_ = 0;
	nConnections_ = 2;
	minZoom_ = 1;
	maxZoom_ = 16;
	flipX_ = false;
	flipY_ = false;
	useQuadKey_ = false;
	stats_ = EMPTY_PROVIDER_STATS;
	lastZoomLevel_ = 0;

	downloadManager_ = new DownloadManager();
	downloadManager_->setMaxConcurentJobs(nConnections_);
	connect(downloadManager_, SIGNAL(finished(DownloadRequestObject*,QNetworkReply*)),
			this, SLOT(downloadReply(DownloadRequestObject*,QNetworkReply*)));
}

TileProvider::~TileProvider()
{
	delete downloadManager_;
}

bool TileProvider::requestTile(int zoom, int x, int y)
{
	QString tileURL = getTileURL(zoom, x, y);
	//TileInfo tileParams = {zoom, x, y};

	if (zoom < minZoom_ || zoom > maxZoom_)
	{
		qDebug() << "Error: Invalid zoom level: " << zoom;
		return false;
	}

	if (lastZoomLevel_ != zoom)
	{
		// Lower priority of pending requests with 3 steps
		downloadManager_->prioritize(3);
		lastZoomLevel_ = zoom;
	}

	// Request download
	DownloadRequestObject * request = new DownloadRequestObject();
	request->zoom = zoom;
	request->x = x;
	request->y = y;
	request->priority = 0;
	request->retries = 2;
	request->url = tileURL;
	downloadManager_->request(request);

	++stats_.numRequestedTiles;

	qDebug() << "Requesting tile " << tileURL;

	return true;
}

void TileProvider::downloadReply(DownloadRequestObject * request, QNetworkReply * reply)
{
	qDebug() << "Reply for: " << request->url;

	stats_.numReceivedBytes += reply->bytesAvailable();

	// Create image
	QImage tileImg;
	if (!tileImg.load(reply, 0))
	{
		qDebug() << "Error creating image";
		char data[200];
		reply->read(data, 200);
		qDebug() << QString(data);
		reply->deleteLater();
		++stats_.numFailedRequests;
		return;
	}

	QPixmap * tile = new QPixmap(QPixmap::fromImage(tileImg));

	// Schedule deletion of reply
	reply->deleteLater();

	// Forward tile
	emit tileReady(request->zoom, request->x, request->y, tile);
	++stats_.numProvidedTiles;
}

void TileProvider::setServerNames(QString names[], int numNames)
{
	if (numNames > 5)
		numNames = 5;

	for (int i = 0; i < numNames; ++i)
	{
		serverNames_[i] = names[i];
	}

	nServers_ = numNames;
}

void TileProvider::setZoomBounds(int minZoom, int maxZoom)
{
	minZoom_ = minZoom;
	maxZoom_ = maxZoom;
}

void TileProvider::setAddressLookupOptions(bool invertX, bool invertY, bool quadKey)
{
	flipX_ = invertX;
	flipY_ = invertY;
	useQuadKey_ = quadKey;
}

void TileProvider::setMaxConcurrentRequests(int maxConnections)
{
	nConnections_ = maxConnections;
}

int TileProvider::getMinZoom()
{
	return minZoom_;
}

int TileProvider::getMaxZoom()
{
	return maxZoom_;
}

QString TileProvider::getTileURL(int zoom, int x, int y)
{
	// Flip x index
	if (flipX_)
	{
		x = (0x1 << zoom) - x - 1;
	}

	// Flip y index
	if (flipY_)
	{
		y = (0x1 << zoom) - y - 1;
	}

	// Quad key coding, used for Bing maps
	if (useQuadKey_)
	{
		QString quadKey;
		for (int i = zoom; i > 0; i--)
		{
			char digit = '0';
			int mask = 1 << (i - 1);
			if ((x & mask) != 0)
			{
				digit++;
			}
			if ((y & mask) != 0)
			{
				digit++;
				digit++;
			}
			quadKey.append(QString(digit));
		}

		return providerUrl_.arg(quadKey).arg(serverNames_[++tileServerCounter_ % nServers_]);
	}
	else
	{
		return providerUrl_.arg(zoom).arg(x).arg(y).arg(serverNames_[++tileServerCounter_ % nServers_]);
	}
}
