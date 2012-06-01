#ifndef TILEPROVIDER_H
#define TILEPROVIDER_H

#include <QObject>
#include <QHash>

// Forward declarations
class DownloadManager;
struct DownloadRequestObject;
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
	uint numReceivedBytes;
};

const ProviderStatistics EMPTY_PROVIDER_STATS = {0, 0, 0, 0};

class TileProvider : public QObject
{
	Q_OBJECT
public:
	explicit TileProvider(QString url, bool invertX = false, bool invertY = false, bool quadKey = false, QObject *parent = 0);
	~TileProvider();

	virtual void requestTile(int zoom, int x, int y);
	ProviderStatistics statistics() { return stats; }

signals:
	void tileReady(int zoom, int x, int y, QPixmap * tile);

public slots:
	void downloadReply(DownloadRequestObject * request, QNetworkReply * reply);

private:
	QString getTileURL(int zoom, int x, int y);

	QString providerUrl;
	bool flipX;
	bool flipY;
	bool useQuadKey;

	DownloadManager * downloadManager;
	QHash<QString, TileInfo> pendingTiles;
	ProviderStatistics stats;
	int tileServerCounter;
};

#endif // TILEPROVIDER_H
