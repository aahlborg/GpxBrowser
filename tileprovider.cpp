#include "tileprovider.h"
#include <QImage>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

static const int tileSize = 256;

//Debug
#include <QDebug>

TileProvider::TileProvider(QObject *parent) :
	QObject(parent),
	stats(EMPTY_PROVIDER_STATS)
{
	networkManager = new QNetworkAccessManager();
	connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReply(QNetworkReply*)));
}

TileProvider::~TileProvider()
{
	delete networkManager;
}

void TileProvider::requestTile(int zoom, int x, int y)
{
	QString tileURL = getTileURL(zoom, x, y);
	TileInfo tileParams = {zoom, x, y};
	pendingTiles.insert(tileURL, tileParams);

	networkManager->get(QNetworkRequest(QUrl(tileURL)));
	++stats.numRequestedTiles;

	qDebug() << "Requesting tile " << tileURL;
}

void TileProvider::networkReply(QNetworkReply * reply)
{
	QString tileURL = reply->url().toString();

	stats.numDownloadedBytes += reply->bytesAvailable();

	// Check for errors
	if (reply->error())
	{
		qDebug() << "Network error: " << reply->error();
		pendingTiles.remove(tileURL);
		reply->deleteLater();
		++stats.numFailedRequests;
		return;
	}
	else if (!pendingTiles.contains(tileURL))
	{
		qDebug() << "Error: No pending request for " << tileURL;
		reply->deleteLater();
		++stats.numFailedRequests;
		return;
	}

	// Take params for this tile
	TileInfo tileParams = pendingTiles.take(tileURL);

	qDebug() << "Reply for: " << tileURL;
	qDebug() << "Pending requests: " << pendingTiles.size();

	// Create image
	QImage tileImg;
	if (!tileImg.load(reply, 0))
	{
		qDebug() << "Error creating image";
		reply->deleteLater();
		++stats.numFailedRequests;
		return;
	}

	QPixmap * tile = new QPixmap(QPixmap::fromImage(tileImg));

	// Schedule deletion of reply
	reply->deleteLater();

	// Forward tile
	emit tileReady(tileParams.zoom, tileParams.x, tileParams.y, tile, this);
	++stats.numProvidedTiles;
}

QString TileProvider::getTileURL(int zoom, int x, int y)
{
	QString sZoom = QString::number(zoom);
	QString sX = QString::number(x);
	QString sY = QString::number(y);

	// URL format
	// %1 zoom
	// %2 x
	// %3 y
	QString googleMapsURL = "http://mt0.google.com/vt/&z=%1&x=%2&y=%3";
	QString googleEarthURL = "http://khm1.google.se/kh/v=111&z=%1&x=%2&s=&y=%3";
	QString googleTerrain1URL = "http://mt0.google.com/vt/lyrs=t@128,r@177186663&z=%1&x=%2&y=%3";
	QString googleTerrain2URL = "http://mt0.google.com/vt/lyrs=t,r&z=%1&x=%2&y=%3";
	QString osmMapnikURL = "http://a.tile.openstreetmap.org/%1/%2/%3.png";
	QString osmCycleMapURL = "http://a.tile.opencyclemap.org/cycle/%1/%2/%3.png";

	return googleMapsURL.arg(zoom).arg(x).arg(y);
}
