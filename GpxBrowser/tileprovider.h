#ifndef TILEPROVIDER_H
#define TILEPROVIDER_H

#include <QObject>
#include <QHash>
#include <QList>

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

struct TileProviderInfo
{
	QString name;
	QString copyright;
	QString url;
	QList<QString> serverList;
	int numConnections;
	int minZoom;
	int maxZoom;
	bool invertX;
	bool invertY;
	bool quadKey;
};

const TileProviderInfo EMPTY_TILE_PROVIDER_INFO = {"", "", "", QList<QString>(), 0, 0, 0, false, false, false};

class TileProvider : public QObject
{
	Q_OBJECT
public:
	explicit TileProvider(TileProviderInfo &info);
	~TileProvider();

	//void setServerNames(QString names[], int numNames);
	void setZoomBounds(int minZoom, int maxZoom);
	void setAddressLookupOptions(bool invertX, bool invertY, bool quadKey);
	void setMaxConcurrentRequests(int maxConnections);

	int getMinZoom();
	int getMaxZoom();

	bool requestTile(int zoom, int x, int y);
	ProviderStatistics statistics() { return stats_; }
	TileProviderInfo info() { return info_; }

signals:
	void tileReady(int zoom, int x, int y, QPixmap * tile);

public slots:
	void downloadReply(DownloadRequestObject * request, QNetworkReply * reply);

private:
	void init();
	QString getTileURL(int zoom, int x, int y);

	TileProviderInfo info_;

	DownloadManager * downloadManager_;
	QHash<QString, TileInfo> pendingTiles_;
	ProviderStatistics stats_;
	int lastZoomLevel_;
	int tileServerCounter_;
};

#endif // TILEPROVIDER_H
