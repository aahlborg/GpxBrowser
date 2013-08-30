/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include "tileprovider.h"
#include <QObject>
#include <QImage>
#include <QHash>

typedef QHash<QString, QPixmap *> TileList;

class TileManager : public QObject
{
	Q_OBJECT
public:
	explicit TileManager(TileProvider * provider);
	~TileManager();

	QPixmap * getTile(int zoom, int x, int y);
	QPixmap * getTile(int zoom, double lat, double lon);
	void getTileIndex(int zoom, double lat, double lon, int &x, int &y);
	void getTileCoord(int zoom, int x, int y, double &lat, double &lon);
	void getTileCoord(int zoom, double x, double y, double &lat, double &lon);
	int getMinZoom();
	int getMaxZoom();

	void purge();

	ProviderStatistics providerStats() { return tileProvider_->statistics(); }
	QString copyrightText() { return tileProvider_->info().copyright; }

	bool debugMode() { return debugMode_; }
	void setDebugMode(bool debugMode) { debugMode_ = debugMode; }

signals:
	void dataUpdated(TileManager * sender, int zoom, int x, int y);

public slots:
	void tileReady(int zoom, int x, int y, QPixmap * tile);

private:
	QPixmap * createTile(int zoom, int x, int y);
	QString getKey(int zoom, int x, int y);

	TileList tiles_;
	TileProvider * tileProvider_;

	bool debugMode_;
};

#endif // TILEMANAGER_H
