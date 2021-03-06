/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtWidgets/QWidget>
#include <QList>
#include <QVector>

class TileManager;
struct TileProviderInfo;

typedef QList<TileProviderInfo *> TileProviderInfoList;
typedef QList<TileManager *> TileManagerList;
typedef QVector<QPointF> CoordinateList;

class MapView : public QWidget
{
	Q_OBJECT
public:
	explicit MapView(QWidget *parent = 0);
	~MapView();

	TileProviderInfoList * getTileProviders() { return &tileProviders_; }
	void addTileProvider(TileProviderInfo &providerInfo);
	void setActiveTileProvider(int index);
	void clearPaths();
	void addPath(QVector<QPointF> &path);
	void clearWaypoints();
	void addWaypoint(QPointF &wpt);
	void setDebugMode(bool debugMode);

	int activeTileProvider() { return activeTileProvider_; }
	bool debugMode() { return debugMode_; }
	
signals:
	
public slots:
	void dataUpdated(TileManager * sender, int zoom, int x, int y);

protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	// Good transformations
	QPointF tileToCoord(const QPointF tile);
	QPointF coordToTile(const QPointF coord);
	QPointF canvasToTile(const QPointF canvas);
	QPointF tileToCanvas(const QPointF tile);
	QPointF canvasToCoord(const QPointF canvas);
	QPointF coordToCanvas(const QPointF coord);

	// Good to have
	QPoint floorPoint(const QPointF pointF);

	bool isActive();

	void drawTiles(QPainter &painter);
	void drawPaths(QPainter &painter);
	void drawPath(QPainter &painter, CoordinateList &path);
	void drawWaypoints(QPainter &painter);
	void drawWaypoint(QPainter &painter, QPointF wpt);
	QPixmap * getWptMarker();

	void moveMap(QPointF delta);
	void changeZoomDelta(const int zoomDelta, const QPointF staticPointCanvas);

	bool mousePressed_;

	// Debug and fun
	bool debugMode_;
	int updateCount_;

	QPointF curCanvas_;
	QPointF oldCanvas_;
	QPointF curTile_;
	QPointF curCoord_;

	// Map specific stuff
	int zoom_;
	QPointF centerCoord_;

	// Tile specific stuff
	TileProviderInfoList tileProviders_;
	TileManagerList tileManagers_;
	int activeTileProvider_;

	// Paths and stuff
	QVector<CoordinateList *> paths_;
	CoordinateList waypoints_;
	QPixmap *wptMarker_;
};

#endif // MAPVIEW_H
