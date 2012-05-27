#include "mapview.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <qmath.h>
#include <cmath>

#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) > (y) ? (y) : (x)

static const double pi = 3.14159265358979;
static const int tileSize = 256;

MapView::MapView(QWidget *parent) :
	QWidget(parent),
	updateCount(0),
	curCanvas(QPointF(0, 0)),
	curTile(QPointF(0, 0)),
	curCoord(QPointF(0, 0)),
	zoom(0),
	centerCoord(QPointF(0, 0)),
	mousePressed(false)
{
	setMouseTracking(true);
	setAutoFillBackground(true);
	connect(&tileManager, SIGNAL(dataUpdated()), this, SLOT(update()));
}

void MapView::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);

	drawTiles(painter);
	int rowCount = 0;

	ProviderStatistics stats = tileManager.providerStats();

	painter.drawText(6, 15 * ++rowCount, QString("Canvas: %1 %2").arg(curCanvas.x()).arg(curCanvas.y()));
	painter.drawText(6, 15 * ++rowCount, QString("Coordinates: %1 %2").arg(curCoord.x()).arg(curCoord.y()));
	painter.drawText(6, 15 * ++rowCount, QString("Zoom: %1").arg(zoom));
	painter.drawText(6, 15 * ++rowCount, QString("Tile: %1 %2").arg(curTile.x()).arg(curTile.y()));
	painter.drawText(6, 15 * ++rowCount, QString("Updates: %1").arg(++updateCount));
	painter.drawText(6, 15 * ++rowCount, QString("Download: %1 MiB").arg(stats.numDownloadedBytes / 1048576.0));
	painter.drawText(6, 15 * ++rowCount, QString("Tiles requested/provided: %1/%2").arg(stats.numRequestedTiles).arg(stats.numProvidedTiles));
}

void MapView::drawTiles(QPainter &painter)
{
	QPointF topLeftCanvas = QPointF(0.0, 0.0);
	QPointF bottomRightCanvas = QPointF(width(), height());
	QPoint firstTile = floorPoint(canvasToTile(topLeftCanvas));
	QPoint lastTile = floorPoint(canvasToTile(bottomRightCanvas));

	// Constraints
	const int n = 0x1 << zoom; // 2^zoom
	firstTile.setX(MAX(0, firstTile.x()));
	firstTile.setY(MAX(0, firstTile.y()));
	lastTile.setX(MIN(n - 1, lastTile.x()));
	lastTile.setY(MIN(n - 1, lastTile.y()));

	for (int tileX = firstTile.x(); tileX <= lastTile.x(); ++tileX)
	{
		for (int tileY = firstTile.y(); tileY <= lastTile.y(); ++tileY)
		{
			QPixmap * tileImg = tileManager.getTile(zoom, tileX, tileY);
			// To floor or not to floor...
			QPointF tilePos = tileToCanvas(QPointF(tileX, tileY));
			painter.drawPixmap(tilePos, *tileImg);
		}
	}
}

void MapView::moveMap(QPointF delta)
{
	const QPointF curCenterCanvas = QPointF(width() / 2, height() / 2);
	const QPointF newCenterCanvas = curCenterCanvas - delta;
	centerCoord = canvasToCoord(newCenterCanvas);
}

void MapView::mouseMoveEvent(QMouseEvent * event)
{
	if (mousePressed)
	{
		oldCanvas = curCanvas;

		curCanvas = QPointF(event->x(), event->y());
		curTile = canvasToTile(curCanvas);
		curCoord = canvasToCoord(curCanvas);

		QPointF delta = curCanvas - oldCanvas;
		moveMap(delta);

		update();
	}
}

void MapView::mousePressEvent(QMouseEvent * event)
{
	mousePressed = true;
	curCanvas = QPointF(event->x(), event->y());
	oldCanvas = curCanvas;
	update();
}

void MapView::mouseReleaseEvent(QMouseEvent * /*event*/)
{
	mousePressed = false;
	update();
}

void MapView::wheelEvent(QWheelEvent * event)
{
	int maxZoom = 16;
	if (event->delta() > 0)
	{
		// Scroll worward, zoom in
		if (zoom < maxZoom)
			++zoom;
	}
	else
	{
		// Scroll backwards, zoom out
		if (zoom > 0)
			--zoom;
	}
	update();
}

QPointF MapView::tileToCoord(const QPointF tile)
{
	const int n = 0x1 << zoom; // 2^zoom

	const double coordX = tile.x() / n * 360.0 - 180.0;
	const double coordY = qAtan(sinh(pi * (1 - 2 * tile.y() / n))) * 180.0 / pi;

	return QPointF(coordX, coordY);
}

QPointF MapView::coordToTile(const QPointF coord)
{
	const int n = 0x1 << zoom; // 2^zoom
	const double radLat = coord.y() * pi / 180.0;

	const double tileX = ((coord.x() + 180.0) / 360.0) * n;
	const double tileY = (1 - (qLn(qTan(radLat) + 1 / qCos(radLat)) / pi)) / 2 * n;

	return QPointF(tileX, tileY);
}

QPointF MapView::canvasToTile(const QPointF canvas) // FEL
{
	const QPointF centerTile = coordToTile(centerCoord);
	const QPointF centerCanvas = QPointF(width() / 2, height() / 2);

	const double canvasX = centerTile.x() + (canvas.x() - centerCanvas.x()) / tileSize;
	const double canvasY = centerTile.y() + (canvas.y() - centerCanvas.y()) / tileSize;

	return QPointF(canvasX, canvasY);
}

QPointF MapView::tileToCanvas(const QPointF tile) // FEL
{
	const QPointF centerTile = coordToTile(centerCoord);
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
