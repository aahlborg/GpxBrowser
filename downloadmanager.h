/** @class DownloadManager
 * A prioritizing download queue
 */

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
//#include <QList>
#include <QHash>

class QNetworkAccessManager;
class QNetworkReply;

// Object containing request info
struct DownloadRequestObject
{
	int zoom;
	int x;            // User info
	int y;
	QString url;      // URL to resource
	int priority;     // Lower is prioritized
	qint64 startTime; // Milliseconds since epoch
	int retries;      // When zero is reached, an error is reported
};

// Statistics
struct DownloadStatistics
{
	uint numRequests;
	uint numProvides;
	uint numFailedRequests;
	uint numRedirects;
	uint numDownloadedBytes;
};

typedef QHash<QString, DownloadRequestObject *> DownloadQueue;
typedef QHashIterator<QString, DownloadRequestObject *> DownloadQueueIterator;

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = 0);
	~DownloadManager();

	void request(DownloadRequestObject * request);
	void prioritize(int delta);

signals:
	void finished(DownloadRequestObject * request, QNetworkReply * reply);

public slots:
	void networkReply(QNetworkReply * reply);
	void pollQueue();
	
private:
	DownloadRequestObject * nextJob();

	// Maximum unmber of simultaneous downloads
	int maxConcurentJobs;
	// Timeout in milliseconds
	int timeout;
	//QList<DownloadRequestObject *> waitingQueue;
	//QList<DownloadRequestObject *> pendingQueue;
	DownloadQueue waitingQueue;
	DownloadQueue pendingQueue;
	QNetworkAccessManager * networkManager;
	DownloadStatistics stats;
};

#endif // DOWNLOADMANAGER_H
