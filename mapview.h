#ifndef MAPVIEW_H
#define MAPVIEW_H

//#include "tilemanager.h"
#include <QWidget>
#include <QList>

class TileManager;

typedef QList<TileManager *> TileManagerList;

class MapView : public QWidget
{
	Q_OBJECT
public:
	explicit MapView(QWidget *parent = 0);
	~MapView();
	
signals:
	
public slots:
	void dataUpdated(TileManager * sender, int zoom, int x, int y);

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

	bool isActive();

	void drawTiles(QPainter &painter);

	void moveMap(QPointF delta);

	bool mousePressed_;

	// Debug and fun
	int updateCount_;

	QPointF curCanvas_;
	QPointF oldCanvas_;
	QPointF curTile_;
	QPointF curCoord_;

	// Map specific stuff
	int zoom_;
	QPointF centerCoord_;

	// Tile specific stuff
	TileManagerList tileManagers_;
	int activeTileManager_;
};

#endif // MAPVIEW_H
