/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#include "mapview.h"
#include "tilemanager.h"
#include "tileprovider.h"
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
	debugMode_(false),
	updateCount_(0),
	curCanvas_(QPointF(0, 0)),
	curTile_(QPointF(0, 0)),
	curCoord_(QPointF(0, 0)),
	zoom_(1),
	centerCoord_(QPointF(0, 0)),
	activeTileProvider_(-1),
	wptMarker_(NULL)
{
	qDebug() << "MapView: Constructor";

	setMouseTracking(true);
	setAutoFillBackground(true);

	QList<QString> serverVariants123; // Google, Bing
	serverVariants123.append("1");
	serverVariants123.append("2");
	serverVariants123.append("3");

	QList<QString> serverVariantsAbc; // OSM
	serverVariantsAbc.append("a");
	serverVariantsAbc.append("b");
	serverVariantsAbc.append("c");

	TileProviderInfo providerInfo;

	// Bing Maps
	providerInfo.name = "Bing Maps";
	providerInfo.copyright = QString::fromLocal8Bit("\u00a9 Bing");
	providerInfo.url = "http://ecn.dynamic.t%4.tiles.virtualearth.net/comp/CompositionHandler/%1?it=G,VE,BX,L,LA&shading=hill";
	providerInfo.serverList.clear();
	providerInfo.serverList.append(serverVariants123);
	providerInfo.numConnections = 8;
	providerInfo.minZoom = 1;
	providerInfo.maxZoom = 18;
	providerInfo.invertX = false;
	providerInfo.invertY = false;
	providerInfo.quadKey = true;
	addTileProvider(providerInfo);

	// Google Maps
	providerInfo.name = "Google Maps";
	providerInfo.copyright = QString::fromLocal8Bit("\u00a9 Google");
	providerInfo.url = "http://mt%4.google.com/vt/&z=%1&x=%2&y=%3";
	providerInfo.serverList.clear();
	providerInfo.serverList.append(serverVariants123);
	providerInfo.numConnections = 8;
	providerInfo.minZoom = 1;
	providerInfo.maxZoom = 18;
	providerInfo.invertX = false;
	providerInfo.invertY = false;
	providerInfo.quadKey = false;
	addTileProvider(providerInfo);

	// Google Aerial
	providerInfo.name = "Google Aerial";
	providerInfo.copyright = QString::fromLocal8Bit("\u00a9 Google");
	providerInfo.url = "http://khm%4.google.se/kh/v=111&z=%1&x=%2&s=&y=%3";
	providerInfo.serverList.clear();
	providerInfo.serverList.append(serverVariants123);
	providerInfo.numConnections = 8;
	providerInfo.minZoom = 1;
	providerInfo.maxZoom = 18;
	providerInfo.invertX = false;
	providerInfo.invertY = false;
	providerInfo.quadKey = false;
	addTileProvider(providerInfo);

	// Google Terrain
	providerInfo.name = "Google Terrain";
	providerInfo.copyright = QString::fromLocal8Bit("\u00a9 Google");
	providerInfo.url = "http://mt%4.google.com/vt/lyrs=t,r&z=%1&x=%2&y=%3";
	providerInfo.serverList.clear();
	providerInfo.serverList.append(serverVariants123);
	providerInfo.numConnections = 8;
	providerInfo.minZoom = 1;
	providerInfo.maxZoom = 15;
	providerInfo.invertX = false;
	providerInfo.invertY = false;
	providerInfo.quadKey = false;
	addTileProvider(providerInfo);

	// OSM Mapnik
	providerInfo.name = "OSM Mapnik";
	providerInfo.copyright = QString::fromLocal8Bit("\u00a9 OpenStreetMap contributors");
	providerInfo.url = "http://%4.tile.openstreetmap.org/%1/%2/%3.png";
	providerInfo.serverList.clear();
	providerInfo.serverList.append(serverVariantsAbc);
	providerInfo.numConnections = 2;
	providerInfo.minZoom = 1;
	providerInfo.maxZoom = 18;
	providerInfo.invertX = false;
	providerInfo.invertY = false;
	providerInfo.quadKey = false;
	addTileProvider(providerInfo);

	// OSM Cyklemap
	providerInfo.name = "OSM Cyclemap";
	providerInfo.copyright = QString::fromLocal8Bit("\u00a9 OpenStreetMap contributors");
	providerInfo.url = "http://%4.tile.opencyclemap.org/cycle/%1/%2/%3.png";
	providerInfo.serverList.clear();
	providerInfo.serverList.append(serverVariantsAbc);
	providerInfo.numConnections = 2;
	providerInfo.minZoom = 1;
	providerInfo.maxZoom = 18;
	providerInfo.invertX = false;
	providerInfo.invertY = false;
	providerInfo.quadKey = false;
	addTileProvider(providerInfo);

	activeTileProvider_ = 0;
}

MapView::~MapView()
{
	qDeleteAll(tileManagers_);
}

bool MapView::isActive()
{
	if (tileManagers_.empty() || activeTileProvider_ < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MapView::addTileProvider(TileProviderInfo &providerInfo)
{
	// Copy info
	TileProviderInfo * newProviderInfo = new TileProviderInfo();
	newProviderInfo->name = providerInfo.name;
	newProviderInfo->copyright = providerInfo.copyright;
	newProviderInfo->url = providerInfo.url;
	newProviderInfo->serverList.append(providerInfo.serverList);
	newProviderInfo->numConnections = providerInfo.numConnections;
	newProviderInfo->minZoom = providerInfo.minZoom;
	newProviderInfo->maxZoom = providerInfo.maxZoom;
	newProviderInfo->invertX = providerInfo.invertX;
	newProviderInfo->invertY = providerInfo.invertY;
	newProviderInfo->quadKey = providerInfo.quadKey;

	// Create TileManager
	TileManager * newTileManager = new TileManager(new TileProvider(*newProviderInfo));
	connect(newTileManager, SIGNAL(dataUpdated(TileManager*,int,int,int)), this, SLOT(dataUpdated(TileManager*,int,int,int)));

	// Add to list
	tileProviders_.append(newProviderInfo);
	tileManagers_.append(newTileManager);
}

void MapView::setActiveTileProvider(int index)
{
	if (index < 0 || index > tileProviders_.size())
	{
		qDebug() << "MapView: Error: Invalid tile provider index";
		return;
	}

	activeTileProvider_ = index;

	// Check zoom level
	if (zoom_ < tileManagers_.at(activeTileProvider_)->getMinZoom())
	{
		zoom_ = tileManagers_.at(activeTileProvider_)->getMinZoom();
	}
	else if (zoom_ > tileManagers_.at(activeTileProvider_)->getMaxZoom())
	{
		zoom_ = tileManagers_.at(activeTileProvider_)->getMaxZoom();
	}

	update();
}

void MapView::setDebugMode(bool debugMode)
{
	debugMode_ = debugMode;
	tileManagers_.at(activeTileProvider_)->setDebugMode(debugMode);
}

void MapView::clearPaths()
{
	qDeleteAll(paths_);
	paths_.clear();
}

void MapView::addPath(QVector<QPointF> &path)
{
	CoordinateList * newPath = new CoordinateList();

	for (int i = 0; i < path.size(); ++i)
	{
		newPath->append(path.at(i));
	}

	paths_.append(newPath);

	update();
}

void MapView::clearWaypoints()
{
	waypoints_.clear();
}

void MapView::addWaypoint(QPointF &wpt)
{
	waypoints_.append(wpt);
	update();
}

void MapView::dataUpdated(TileManager * sender, int /*zoom*/, int /*x*/, int /*y*/)
{
	if (isActive() && sender == tileManagers_.at(activeTileProvider_))
	{
		update();
	}
}

void MapView::paintEvent(QPaintEvent * /*event*/)
{
	if (!isActive())
		return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	drawTiles(painter);
	drawPaths(painter);
	drawWaypoints(painter);

	if (debugMode_)
	{
		ProviderStatistics stats = tileManagers_.at(activeTileProvider_)->providerStats();
		int rowCount = 0;
		painter.drawText(6, 15 * ++rowCount, QString("Canvas: %1 %2").arg(curCanvas_.x()).arg(curCanvas_.y()));
		painter.drawText(6, 15 * ++rowCount, QString("Coordinates: %1 %2").arg(curCoord_.x()).arg(curCoord_.y()));
		painter.drawText(6, 15 * ++rowCount, QString("Zoom: %1").arg(zoom_));
		painter.drawText(6, 15 * ++rowCount, QString("Tile: %1 %2").arg(curTile_.x()).arg(curTile_.y()));
		painter.drawText(6, 15 * ++rowCount, QString("Updates: %1").arg(++updateCount_));
		painter.drawText(6, 15 * ++rowCount, QString("Download: %1 MiB").arg(stats.numReceivedBytes / 1048576.0));
		painter.drawText(6, 15 * ++rowCount, QString("Tiles requested/provided: %1/%2").arg(stats.numRequestedTiles).arg(stats.numProvidedTiles));
	}
}

void MapView::drawTiles(QPainter &painter)
{
	QPointF topLeftCanvas = QPointF(0.0, 0.0);
	QPointF bottomRightCanvas = QPointF(width(), height());
	QPoint firstTile = floorPoint(canvasToTile(topLeftCanvas));
	QPoint lastTile = floorPoint(canvasToTile(bottomRightCanvas));

	// Constraints
	const long n = 0x1 << zoom_; // 2^zoom
	firstTile.setX(MAX(0, firstTile.x()));
	firstTile.setY(MAX(0, firstTile.y()));
	lastTile.setX(MIN(n - 1, lastTile.x()));
	lastTile.setY(MIN(n - 1, lastTile.y()));

	for (int tileX = firstTile.x(); tileX <= lastTile.x(); ++tileX)
	{
		for (int tileY = firstTile.y(); tileY <= lastTile.y(); ++tileY)
		{
			QPixmap * tileImg = tileManagers_.at(activeTileProvider_)->getTile(zoom_, tileX, tileY);
			if (NULL != tileImg)
			{
				// To floor or not to floor...
				QPointF tilePos = tileToCanvas(QPointF(tileX, tileY));
				painter.drawPixmap(tilePos, *tileImg);
			}
		}
	}

	// Draw copyright text
	painter.drawText(6, height() - 8, tileManagers_.at(activeTileProvider_)->copyrightText());
}

void MapView::drawPaths(QPainter &painter)
{
	for (int i = 0; i < paths_.size(); ++i)
	{
		drawPath(painter, *(paths_.at(i)));
	}
}

void MapView::drawPath(QPainter &painter, CoordinateList &path)
{
	if (path.size() < 2)
	{
		return;
	}

	// Convert points to canvas coordinates
	QVector<QPointF> canvasPath;
	for (int i = 0; i < path.size(); ++i)
	{
		canvasPath.append(coordToCanvas(path.at(i)));
	}

	// Construct lines
	QVector<QLineF> pathLines;
	for (int i = 0; i < canvasPath.size() - 1; ++i)
	{
		pathLines.append(QLineF(canvasPath.at(i), canvasPath.at(i + 1)));
	}

	painter.save();
	painter.setPen(QPen(QBrush(Qt::red), 2));
	painter.drawLines(pathLines);
	painter.restore();
}

void MapView::drawWaypoints(QPainter &painter)
{
	for (int i = 0; i < waypoints_.size(); ++i)
	{
		drawWaypoint(painter, waypoints_.at(i));
	}
}

void MapView::drawWaypoint(QPainter &painter, QPointF wpt)
{
	const QPixmap * wptMarker = getWptMarker();
	const QPointF wptCanvas = coordToCanvas(wpt);
	const QSize halfSize = wptMarker->size() / 2;
	const QPointF origin(wptCanvas.x() - halfSize.width(), wptCanvas.y() - halfSize.height());

	painter.drawPixmap(origin, *wptMarker);
}

QPixmap * MapView::getWptMarker()
{
	if (NULL == wptMarker_)
	{
		// Create one
		const int size = 16;
		QImage markerImg(QSize(size, size), QImage::Format_ARGB32);
		markerImg.fill(qRgba(0, 0, 0, 0));
		QPainter painter(&markerImg);
		painter.setRenderHint(QPainter::Antialiasing);

		// Draw
		painter.setPen(QPen(QBrush(Qt::gray), 1.2));
		painter.setBrush(Qt::white);
		painter.drawEllipse(QPoint(size / 2, size / 2), 6, 6);

		painter.setPen(QPen(QBrush(Qt::gray), 1));
		painter.setBrush(QColor(79, 155, 255));
		painter.drawEllipse(QPoint(size / 2, size / 2), 3, 3);

		// Create pixmap
		wptMarker_ = new QPixmap(QPixmap::fromImage(markerImg));
	}

	return wptMarker_;
}

void MapView::moveMap(QPointF delta)
{
	const QPointF curCenterCanvas = QPointF(width() / 2, height() / 2);
	const QPointF newCenterCanvas = curCenterCanvas - delta;
	centerCoord_ = canvasToCoord(newCenterCanvas);
}

void MapView::changeZoomDelta(const int zoomDelta, const QPointF staticPointCanvas)
{
	if (zoom_ + zoomDelta > tileManagers_.at(activeTileProvider_)->getMaxZoom() ||
		zoom_ + zoomDelta < tileManagers_.at(activeTileProvider_)->getMinZoom())
	{
		// Invalid zoom level
		return;
	}

	// The static and center canvas points are invariant during zoom
	const QPointF centerCanvas = QPointF(width() / 2, height() / 2);
	// These transformations are made before changing the zoom and center coordinate
	const QPointF staticPointTileOld = canvasToTile(staticPointCanvas);
	const QPointF centerTileOld = canvasToTile(centerCanvas);
	// This relation will be valid for both the old and the new zoom
	const QPointF staticPointCenterTile = centerTileOld - staticPointTileOld;
	// Change zoom now
	zoom_ += zoomDelta;
	// The new tile can be calculated in this manner
	const QPointF pointerTileNew = qPow(2, zoomDelta) * staticPointTileOld;
	// The new center is the same number of tiles away
	const QPointF centerTileNew = pointerTileNew + staticPointCenterTile;
	// Tile <-> coordinate transformation uses the new zoom value,
	// but doesn't rely on the center coordinate, which is being calculated
	centerCoord_ = tileToCoord(centerTileNew);

	update();
}

void MapView::mouseMoveEvent(QMouseEvent *event)
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

void MapView::mousePressEvent(QMouseEvent *event)
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

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePressed_ = false;
		update();
	}
}

void MapView::wheelEvent(QWheelEvent *event)
{
	int zoomDelta = 0;

	if (event->delta() > 0)
	{
		// Scroll forward, zoom in
		zoomDelta = 1;
	}
	else
	{
		// Scroll backwards, zoom out
		zoomDelta = -1;
	}

	// Change zoom, keeping the mouse pointer position static
	changeZoomDelta(zoomDelta, QPointF(event->pos()));
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
	// Zoom in one step, keeping the mouse pointer position static
	changeZoomDelta(1, QPointF(event->pos()));
}

QPointF MapView::tileToCoord(const QPointF tile)
{
	const long n = 0x1 << zoom_; // 2^zoom

	const double coordX = tile.x() / n * 360.0 - 180.0;
	const double coordY = qAtan(sinh(pi * (1 - 2 * tile.y() / n))) * 180.0 / pi;

	return QPointF(coordX, coordY);
}

QPointF MapView::coordToTile(const QPointF coord)
{
	const long n = 0x1 << zoom_; // 2^zoom
	const double radLat = coord.y() * pi / 180.0;

	const double tileX = ((coord.x() + 180.0) / 360.0) * n;
	const double tileY = (1 - (qLn(qTan(radLat) + 1 / qCos(radLat)) / pi)) / 2 * n;

	return QPointF(tileX, tileY);
}

QPointF MapView::canvasToTile(const QPointF canvas)
{
	const QPointF centerTile = coordToTile(centerCoord_);
	const QPointF centerCanvas = QPointF(width() / 2, height() / 2);

	const double canvasX = centerTile.x() + (canvas.x() - centerCanvas.x()) / tileSize;
	const double canvasY = centerTile.y() + (canvas.y() - centerCanvas.y()) / tileSize;

	return QPointF(canvasX, canvasY);
}

QPointF MapView::tileToCanvas(const QPointF tile)
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
