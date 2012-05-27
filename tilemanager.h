#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QObject>
#include <QImage>
#include <QHash>
#include "tileprovider.h"

class TileManager : public QObject
{
	Q_OBJECT
public:
	explicit TileManager(QObject *parent = 0);
	~TileManager();

	QPixmap * getTile(int zoom, int x, int y);
	QPixmap * getTile(int zoom, double lat, double lon);
	void getTileIndex(int zoom, double lat, double lon, int &x, int &y);
	void getTileCoord(int zoom, int x, int y, double &lat, double &lon);
	void getTileCoord(int zoom, double x, double y, double &lat, double &lon);

	void purge();

	ProviderStatistics providerStats() { return tileProvider.statistics(); }

signals:
	void dataUpdated();

public slots:
	void tileReady(int zoom, int x, int y, QPixmap * tile, TileProvider * sender);

private:
	QPixmap * createTile(int zoom, int x, int y);
	QString getKey(int zoom, int x, int y);

	QHash<QString, QPixmap *> tiles;
	TileProvider tileProvider;
};

#endif // TILEMANAGER_H
