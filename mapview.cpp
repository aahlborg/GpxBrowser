#include "mapview.h"
#include "tilemanager.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <qmath.h>
#include <cmath>
#include <QtAlgorithms>

#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) > (y) ? (y) : (x)

static const double pi = 3.14159265358979323846;
static const int tileSize = 256;

MapView::MapView(QWidget *parent) :
	QWidget(parent),
	mousePressed_(false),
	updateCount_(0),
	curCanvas_(QPointF(0, 0)),
	curTile_(QPointF(0, 0)),
	curCoord_(QPointF(0, 0)),
	zoom_(1),
	centerCoord_(QPointF(0, 0)),
	activeTileManager_(-1)
{
	setMouseTracking(true);
	setAutoFillBackground(true);


	//QString serverVariants[] = {"a", "b", "c"}; // OSM
	QString serverVariants[] = {"1", "2", "3"}; // Google, Bing, Eniro

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

	// Bing map
	QString bingMapURL = "http://ecn.dynamic.t%4.tiles.virtualearth.net/comp/CompositionHandler/%1?it=G,VE,BX,L,LA&shading=hill";
	TileProvider * bingMapProvider = new TileProvider(bingMapURL);
	bingMapProvider->setServerNames(serverVariants, 3);
	bingMapProvider->setAddressLookupOptions(false, false, true);
	bingMapProvider->setZoomBounds(1, 18);
	bingMapProvider->setMaxConcurrentRequests(8);

	tileManagers_.append(new TileManager(bingMapProvider));
	activeTileManager_ = 0;
	connect(tileManagers_[0], SIGNAL(dataUpdated(TileManager*,int,int,int)), this, SLOT(dataUpdated(TileManager*,int,int,int)));
}

MapView::~MapView()
{
	qDeleteAll(tileManagers_);
}

bool MapView::isActive()
{
	if (tileManagers_.empty() || activeTileManager_ < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MapView::dataUpdated(TileManager * sender, int /*zoom*/, int /*x*/, int /*y*/)
{
	if (isActive() && sender == tileManagers_.at(activeTileManager_))
	{
		update();
	}
}

void MapView::paintEvent(QPaintEvent * /*event*/)
{
	if (!isActive())
		return;

	QPainter painter(this);

	drawTiles(painter);
	int rowCount = 0;

	ProviderStatistics stats = tileManagers_.at(activeTileManager_)->providerStats();

	painter.drawText(6, 15 * ++rowCount, QString("Canvas: %1 %2").arg(curCanvas_.x()).arg(curCanvas_.y()));
	painter.drawText(6, 15 * ++rowCount, QString("Coordinates: %1 %2").arg(curCoord_.x()).arg(curCoord_.y()));
	painter.drawText(6, 15 * ++rowCount, QString("Zoom: %1").arg(zoom_));
	painter.drawText(6, 15 * ++rowCount, QString("Tile: %1 %2").arg(curTile_.x()).arg(curTile_.y()));
	painter.drawText(6, 15 * ++rowCount, QString("Updates: %1").arg(++updateCount_));
	painter.drawText(6, 15 * ++rowCount, QString("Download: %1 MiB").arg(stats.numReceivedBytes / 1048576.0));
	painter.drawText(6, 15 * ++rowCount, QString("Tiles requested/provided: %1/%2").arg(stats.numRequestedTiles).arg(stats.numProvidedTiles));
}

void MapView::drawTiles(QPainter &painter)
{
	QPointF topLeftCanvas = QPointF(0.0, 0.0);
	QPointF bottomRightCanvas = QPointF(width(), height());
	QPoint firstTile = floorPoint(canvasToTile(topLeftCanvas));
	QPoint lastTile = floorPoint(canvasToTile(bottomRightCanvas));

	// Constraints
	const int n = 0x1 << zoom_; // 2^zoom
	firstTile.setX(MAX(0, firstTile.x()));
	firstTile.setY(MAX(0, firstTile.y()));
	lastTile.setX(MIN(n - 1, lastTile.x()));
	lastTile.setY(MIN(n - 1, lastTile.y()));

	for (int tileX = firstTile.x(); tileX <= lastTile.x(); ++tileX)
	{
		for (int tileY = firstTile.y(); tileY <= lastTile.y(); ++tileY)
		{
			QPixmap * tileImg = tileManagers_.at(activeTileManager_)->getTile(zoom_, tileX, tileY);
			if (NULL != tileImg)
			{
				// To floor or not to floor...
				QPointF tilePos = tileToCanvas(QPointF(tileX, tileY));
				painter.drawPixmap(tilePos, *tileImg);
			}
		}
	}
}

void MapView::moveMap(QPointF delta)
{
	const QPointF curCenterCanvas = QPointF(width() / 2, height() / 2);
	const QPointF newCenterCanvas = curCenterCanvas - delta;
	centerCoord_ = canvasToCoord(newCenterCanvas);
}

void MapView::mouseMoveEvent(QMouseEvent * event)
{
	if (mousePressed_)
	{
		oldCanvas_ = curCanvas_;

		curCanvas_ = QPointF(event->x(), event->y());
		curTile_ = canvasToTile(curCanvas_);
		curCoord_ = canvasToCoord(curCanvas_);

		QPointF delta = curCanvas_ - oldCanvas_;
		moveMap(delta);

		update();
	}
}

void MapView::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePressed_ = true;
		curCanvas_ = QPointF(event->x(), event->y());
		oldCanvas_ = curCanvas_;
		curTile_ = canvasToTile(curCanvas_);
		curCoord_ = canvasToCoord(curCanvas_);
		update();
	}
}

void MapView::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePressed_ = false;
		update();
	}
}

void MapView::wheelEvent(QWheelEvent * event)
{
	int zoomDelta = 0;

	if (event->delta() > 0)
	{
		// Scroll forward, zoom in
		if (zoom_ >= tileManagers_.at(activeTileManager_)->getMaxZoom())
			return;
		// Flag new zoom level but don't change yet
		zoomDelta = 1;
	}
	else
	{
		// Scroll backwards, zoom out
		if (zoom_ <= tileManagers_.at(activeTileManager_)->getMinZoom())
			return;
		// Flag new zoom level but don't change yet
		zoomDelta = -1;
	}

	// Calculate new center coordinate, keeping coordinate under pointer constant

	// These canvas points are invariant during zoom
	const QPointF pointerCanvas = QPointF(event->pos());
	const QPointF centerCanvas = QPointF(width() / 2, height() / 2);
	// These transformations are made before changing the zoom and center coordinate
	const QPointF pointerTileOld = canvasToTile(pointerCanvas);
	const QPointF centerTileOld = canvasToTile(centerCanvas);
	// This relation will be valid for both the old and the new zoom
	const QPointF pointerCenterTile = centerTileOld - pointerTileOld;
	// Change zoom now
	zoom_ += zoomDelta;
	// The new tile can be calculated in this manner
	const QPointF pointerTileNew = qPow(2, zoomDelta) * pointerTileOld;
	// The new center is the same number of tiles away
	const QPointF centerTileNew = pointerTileNew + pointerCenterTile;
	// Tile <-> coordinate transformation uses the new zoom value,
	// but doesn't rely on the center coordinate, which is being calculated
	centerCoord_ = tileToCoord(centerTileNew);

	update();
}

QPointF MapView::tileToCoord(const QPointF tile)
{
	const int n = 0x1 << zoom_; // 2^zoom

	const double coordX = tile.x() / n * 360.0 - 180.0;
	const double coordY = qAtan(sinh(pi * (1 - 2 * tile.y() / n))) * 180.0 / pi;

	return QPointF(coordX, coordY);
}

QPointF MapView::coordToTile(const QPointF coord)
{
	const int n = 0x1 << zoom_; // 2^zoom
	const double radLat = coord.y() * pi / 180.0;

	const double tileX = ((coord.x() + 180.0) / 360.0) * n;
	const double tileY = (1 - (qLn(qTan(radLat) + 1 / qCos(radLat)) / pi)) / 2 * n;

	return QPointF(tileX, tileY);
}

QPointF MapView::canvasToTile(const QPointF canvas) // FEL
{
	const QPointF centerTile = coordToTile(centerCoord_);
	const QPointF centerCanvas = QPointF(width() / 2, height() / 2);

	const double canvasX = centerTile.x() + (canvas.x() - centerCanvas.x()) / tileSize;
	const double canvasY = centerTile.y() + (canvas.y() - centerCanvas.y()) / tileSize;

	return QPointF(canvasX, canvasY);
}

QPointF MapView::tileToCanvas(const QPointF tile) // FEL
{
	const QPointF centerTile = coordToTile(centerCoord_);
	const QPointF centerCanvas = QPointF(width() / 2, height() / 2);

	const double tileX = centerCanvas.x() + tileSize * (tile.x() - centerTile.x());
	const double tileY = centerCanvas.y() + tileSize * (tile.y() - centerTile.y());

	return QPointF(tileX, tileY);
}

QPointF MapView::canvasToCoord(const QPointF canvas)
{
	return tileToCoord(canvasToTile(canvas));
}

QPointF MapView::coordToCanvas(const QPointF coord)
{
	return tileToCanvas(coordToTile(coord));
}

QPoint MapView::floorPoint(const QPointF pointF)
{
	return QPoint(qFloor(pointF.x()), qFloor(pointF.y()));
}
