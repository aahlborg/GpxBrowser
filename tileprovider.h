#ifndef TILEPROVIDER_H
#define TILEPROVIDER_H

#include <QObject>
#include <QHash>

class QNetworkAccessManager;
class QNetworkReply;

class QPixmap;

struct TileInfo
{
	int zoom;
	int x;
	int y;
};

struct ProviderStatistics
{
	uint numRequestedTiles;
	uint numProvidedTiles;
	uint numFailedRequests;
	uint numDownloadedBytes;
};

const ProviderStatistics EMPTY_PROVIDER_STATS = {0, 0, 0, 0};

class TileProvider : public QObject
{
	Q_OBJECT
public:
	explicit TileProvider(QObject *parent = 0);
	~TileProvider();

	virtual void requestTile(int zoom, int x, int y);
	ProviderStatistics statistics() { return stats; }

	
signals:
	void tileReady(int zoom, int x, int y, QPixmap * tile, TileProvider * sender);

public slots:
	void networkReply(QNetworkReply * reply);

private:
	QString getTileURL(int zoom, int x, int y);

	QNetworkAccessManager * networkManager;
	QHash<QString, TileInfo> pendingTiles;
	ProviderStatistics stats;
};

#endif // TILEPROVIDER_H
