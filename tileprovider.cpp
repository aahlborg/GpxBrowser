#include "tileprovider.h"
#include <QImage>
#include <QPixmap>
#include <QNetworkReply>
#include "downloadmanager.h"

static const int tileSize = 256;

//Debug
#include <QDebug>

TileProvider::TileProvider(QString url, bool invertX, bool invertY, bool quadKey, QObject *parent) :
	QObject(parent),
	providerUrl(url),
	flipX(invertX),
	flipY(invertY),
	useQuadKey(quadKey),
	stats(EMPTY_PROVIDER_STATS),
	tileServerCounter(0)
{
	downloadManager = new DownloadManager();
	connect(downloadManager, SIGNAL(finished(DownloadRequestObject*,QNetworkReply*)),
			this, SLOT(downloadReply(DownloadRequestObject*,QNetworkReply*)));
}

TileProvider::~TileProvider()
{
	delete downloadManager;
}

void TileProvider::requestTile(int zoom, int x, int y)
{
	QString tileURL = getTileURL(zoom, x, y);
	//TileInfo tileParams = {zoom, x, y};

	// Request download
	DownloadRequestObject * request = new DownloadRequestObject();
	request->zoom = zoom;
	request->x = x;
	request->y = y;
	request->priority = 0;
	request->retries = 2;
	request->url = tileURL;
	downloadManager->request(request);

	++stats.numRequestedTiles;

	qDebug() << "Requesting tile " << tileURL;
}

void TileProvider::downloadReply(DownloadRequestObject * request, QNetworkReply * reply)
{
	qDebug() << "Reply for: " << request->url;

	stats.numReceivedBytes += reply->bytesAvailable();

	// Create image
	QImage tileImg;
	if (!tileImg.load(reply, 0))
	{
		qDebug() << "Error creating image";
		char data[200];
		reply->read(data, 200);
		qDebug() << QString(data);
		reply->deleteLater();
		++stats.numFailedRequests;
		return;
	}

	QPixmap * tile = new QPixmap(QPixmap::fromImage(tileImg));

	// Schedule deletion of reply
	reply->deleteLater();

	// Forward tile
	emit tileReady(request->zoom, request->x, request->y, tile);
	++stats.numProvidedTiles;
}

QString TileProvider::getTileURL(int zoom, int x, int y)
{
	//QString serverVariants[] = {"a", "b", "c"}; // OSM
	QString serverVariants[] = {"1", "2", "3"}; // Google, Bing

	// URL format
	// %1 zoom
	// %2 x
	// %3 y
	// %4 server
	QString googleMapsURL = "http://mt%4.google.com/vt/&z=%1&x=%2&y=%3";
	QString googleEarthURL = "http://khm%4.google.se/kh/v=111&z=%1&x=%2&s=&y=%3";
	QString googleTerrain1URL = "http://mt%4.google.com/vt/lyrs=t@128,r@177186663&z=%1&x=%2&y=%3";
	QString googleTerrain2URL = "http://mt%4.google.com/vt/lyrs=t,r&z=%1&x=%2&y=%3";
	// Open street map Mapnik, servers: a-c
	QString osmMapnikURL = "http://%4.tile.openstreetmap.org/%1/%2/%3.png";
	// Open sycle map, servers: a-c
	QString osmCycleMapURL = "http://%4.tile.opencyclemap.org/cycle/%1/%2/%3.png";
	// Topological shadows for overlaying osm maps
	QString topologyTilesURL = "http://toolserver.org/~cmarqu/hill/%1/%2/%3.png";
	// Eniro nautical, servers: 01-06
	QString eniroNauticalURL = "http://map0%4.eniro.no/geowebcache/service/tms1.0.0/nautical/%1/%2/%3.png";
	// Eniro map, servers: 01-06
	QString eniroMapURL = "http://map0%4.eniro.no/geowebcache/service/tms1.0.0/map/%1/%2/%3.png";
	// Eniro aerial, servers: 01-06
	QString eniroAerialURL = "http://map0%4.eniro.no/geowebcache/service/tms1.0.0/aerial/%1/%2/%3.jpeg";
	// Hitta.se map
	QString hittaMapURL = "http://bf.static.hitta.se/tile/v1/0/%1/%2/%3";
	QString hittaAerialURL = "http://bf.static.hitta.se/tile/v1/1/%1/%2/%3";

	// Flip y index for eniro
	//y = (0x1 << zoom) - y - 1;

	//return hittaMapURL.arg(zoom).arg(x).arg(y).arg(serverVariants[++tileServerCounter % 3]);

	// Quad key for Bing
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

	QString bingMapURL = "http://ecn.dynamic.t%4.tiles.virtualearth.net/comp/CompositionHandler/%1?it=G,VE,BX,L,LA&shading=hill";
	return bingMapURL.arg(quadKey).arg(serverVariants[++tileServerCounter % 3]);
}
