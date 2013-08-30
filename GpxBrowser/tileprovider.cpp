/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#include "tileprovider.h"
#include <QImage>
#include <QPixmap>
#include <QNetworkReply>
#include "downloadmanager.h"

static const int tileSize = 256;

//Debug
#include <QDebug>

TileProvider::TileProvider(TileProviderInfo &info) :
	QObject(NULL)
{
	init();

	// Copy TileProviderInfo
	info_.name = info.name;
	info_.copyright = info.copyright;
	info_.url = info.url;
	info_.serverList = QList<QString>(info.serverList);
	info_.numConnections = info.numConnections;
	info_.minZoom = info.minZoom;
	info_.maxZoom = info.maxZoom;
	info_.invertX = info.invertX;
	info_.invertY = info.invertY;
	info_.quadKey = info.quadKey;

	downloadManager_->setMaxConcurentJobs(info_.numConnections);
}

void TileProvider::init()
{
	// Set default values
	info_ = EMPTY_TILE_PROVIDER_INFO;
	stats_ = EMPTY_PROVIDER_STATS;
	lastZoomLevel_ = 0;
	tileServerCounter_ = 0;

	downloadManager_ = new DownloadManager();
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

	if (zoom < info_.minZoom || zoom > info_.maxZoom)
	{
		qDebug() << "TileProvider: Error: Invalid zoom level: " << zoom;
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

	qDebug() << "TileProvider: Requesting tile " << tileURL;

	return true;
}

void TileProvider::downloadReply(DownloadRequestObject * request, QNetworkReply * reply)
{
	qDebug() << "TileProvider: Reply for: " << request->url;

	stats_.numReceivedBytes += reply->bytesAvailable();

	// Create image
	QImage tileImg;
	if (!tileImg.load(reply, 0))
	{
		qDebug() << "TileProvider: Error creating image";
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

void TileProvider::setZoomBounds(int minZoom, int maxZoom)
{
	info_.minZoom = minZoom;
	info_.maxZoom = maxZoom;
}

void TileProvider::setAddressLookupOptions(bool invertX, bool invertY, bool quadKey)
{
	info_.invertX = invertX;
	info_.invertY = invertY;
	info_.quadKey = quadKey;
}

void TileProvider::setMaxConcurrentRequests(int maxConnections)
{
	info_.numConnections = maxConnections;
}

int TileProvider::getMinZoom()
{
	return info_.minZoom;
}

int TileProvider::getMaxZoom()
{
	return info_.maxZoom;
}

QString TileProvider::getTileURL(int zoom, int x, int y)
{
	// Flip x index
	if (info_.invertX)
	{
		x = (0x1 << zoom) - x - 1;
	}

	// Flip y index
	if (info_.invertY)
	{
		y = (0x1 << zoom) - y - 1;
	}

	// Quad key coding, used for Bing maps
	if (info_.quadKey)
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

		QString url = info_.url.arg(quadKey);
		if (!info_.serverList.isEmpty())
		{
			url = url.arg(info_.serverList.at(tileServerCounter_++ % info_.serverList.size()));
		}
		return url;
	}
	else
	{
		QString url = info_.url.arg(zoom).arg(x).arg(y);
		if (!info_.serverList.isEmpty())
		{
			url = url.arg(info_.serverList.at(tileServerCounter_++ % info_.serverList.size()));
		}
		return url;
	}
}
