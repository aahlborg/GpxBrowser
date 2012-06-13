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

	void setServerNames(QString names[], int numNames);
	void setZoomBounds(int minZoom, int maxZoom);
	void setAddressLookupOptions(bool invertX, bool invertY, bool quadKey);
	void setMaxConcurrentRequests(int maxConnections);

	int getMinZoom();
	int getMaxZoom();

	bool requestTile(int zoom, int x, int y);
	ProviderStatistics statistics() { return stats_; }

signals:
	void tileReady(int zoom, int x, int y, QPixmap * tile);

public slots:
	void downloadReply(DownloadRequestObject * request, QNetworkReply * reply);

private:
	void init();
	QString getTileURL(int zoom, int x, int y);

	QString providerUrl_;
	QString serverNames_[5];
	int nServers_;
	int tileServerCounter_;
	int nConnections_;
	int minZoom_;
	int maxZoom_;
	bool flipX_;
	bool flipY_;
	bool useQuadKey_;

	DownloadManager * downloadManager_;
	QHash<QString, TileInfo> pendingTiles_;
	ProviderStatistics stats_;
	int lastZoomLevel_;
};

#endif // TILEPROVIDER_H
