#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "tilemanager.h"
#include <QWidget>
#include <QVector>

class MapView : public QWidget
{
	Q_OBJECT
public:
	explicit MapView(QWidget *parent = 0);
	
signals:
	
public slots:

protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);

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

	void drawTiles(QPainter &painter);

	void moveMap(QPointF delta);

	bool mousePressed;

	// Debug and fun
	int updateCount;

	QPointF curCanvas;
	QPointF oldCanvas;
	QPointF curTile;
	QPointF curCoord;

	// Map specific stuff
	int zoom;
	QPointF centerCoord;

	// Tile specific stuff
	TileManager tileManager;
};

#endif // MAPVIEW_H
