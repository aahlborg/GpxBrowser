#include "tilemanager.h"
#include <qmath.h>
#include <cmath>
#include <QtAlgorithms>
#include <QPainter>

// Debug
#include <QDebug>

static const double pi = 3.14159265358979;
static const int tileSize = 256;

TileManager::TileManager(QObject *parent) :
    QObject(parent)
{
	connect(&tileProvider, SIGNAL(tileReady(int,int,int,QPixmap*,TileProvider*)), this, SLOT(tileReady(int,int,int,QPixmap*,TileProvider*)));
}

TileManager::~TileManager()
{
	purge();
}

QPixmap * TileManager::getTile(int zoom, int x, int y)
{
	// Check values
	int n = qPow(2, zoom);
	if (zoom < 0 || x < 0 || x >= n || y < 0 || y >= n)
	{
		return NULL;
	}

	// Request cached tile
	QPixmap * tile = tiles.value(getKey(zoom, x, y));

	// Create one if neccessary
	if (NULL == tile)
	{
		// Create temporary
		tile = createTile(zoom, x, y);
		tiles.insert(getKey(zoom, x, y), tile);

		// Request tile
		tileProvider.requestTile(zoom, x, y);
	}

	return tile;
}

QPixmap * TileManager::getTile(int zoom, double lat, double lon)
{
	int x, y;

	getTileIndex(zoom, lat, lon, x, y);

	return getTile(zoom, x, y);
}

void TileManager::getTileIndex(int zoom, double lat, double lon, int &x, int &y)
{
	int n = qPow(2, zoom);

	x = qFloor(((lon + 180.0) / 360.0) * n);
	y = qFloor((1.0 - (qLn(qTan(lat * pi / 180.0) + (1 / qCos(lat * pi / 180))) / pi)) / 2 * n);
}

void TileManager::getTileCoord(int zoom, int x, int y, double &lat, double &lon)
{
	getTileCoord(zoom, static_cast<double>(x), static_cast<double>(y), lat, lon);
}

void TileManager::getTileCoord(int zoom, double x, double y, double &lat, double &lon)
{
	int n = qPow(2, zoom);

	lon = x / n * 360.0 - 180.0;
	lat = qAtan(sinh(pi * (1 - 2 * y / n))) * 180.0 / pi;
}

void TileManager::purge()
{
	qDeleteAll(tiles);
	tiles.clear();
}

void TileManager::tileReady(int zoom, int x, int y, QPixmap * tile, TileProvider * sender)
{
	// Replace tile in cache
	if (NULL != tile)
	{
		// Remove old tile
		QPixmap * oldTile = tiles.take(getKey(zoom, x, y));
		delete oldTile;
		oldTile = NULL;

		tiles.insert(getKey(zoom, x, y), tile);
	}

	qDebug() << "Got tile " << zoom << " " << x << " " << y;

	// Signal an update
	emit dataUpdated();
}

QPixmap * TileManager::createTile(int zoom, int x, int y)
{
	QImage tileImg(tileSize, tileSize, QImage::Format_RGB32);
	QPainter painter(&tileImg);

	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::SolidLine);

	painter.drawText(100, 80, "zoom: " + QString::number(zoom));
	painter.drawText(100, 100, "x: " + QString::number(x));
	painter.drawText(100, 120, "y: " + QString::number(y));

	painter.drawRect(0, 0, tileSize, tileSize);

	QPixmap * tile = new QPixmap(QPixmap::fromImage(tileImg));

	return tile;
}

QString TileManager::getKey(int zoom, int x, int y)
{
	return QString::number(zoom) + ":" + QString::number(x) + ":" + QString::number(y);
}
