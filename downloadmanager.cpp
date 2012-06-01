#include "downloadmanager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>

// Debug
#include <QDebug>

DownloadManager::DownloadManager(QObject *parent) :
	QObject(parent),
	maxConcurentJobs(8),
	timeout(10000)
{
	networkManager = new QNetworkAccessManager();
	connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReply(QNetworkReply*)));
}

DownloadManager::~DownloadManager()
{
	delete networkManager;
}

void DownloadManager::request(DownloadRequestObject * request)
{
	waitingQueue.insert(request->url, request);
	pollQueue();
}

void DownloadManager::prioritize(int delta)
{
	// Change priority for all items in queue
	//for (int i = 0; i < waitingQueue.size(); ++i)
	DownloadQueueIterator i(waitingQueue);
	while (i.hasNext())
	{
		//waitingQueue.at(i).priority += delta;
		i.next();
		i.value()->priority += delta;
	}
}

void DownloadManager::networkReply(QNetworkReply * reply)
{
	QString url = reply->url().toString();

	stats.numDownloadedBytes += reply->bytesAvailable();

	// Check for errors
	if (reply->error())
	{
		qDebug() << "Network error: " << reply->error();
		pendingQueue.remove(url);
		reply->deleteLater();
		++stats.numFailedRequests;
		return;
	}
	else if (!pendingQueue.contains(url))
	{
		qDebug() << "Error: No pending request for " << url;
		reply->deleteLater();
		++stats.numFailedRequests;
		return;
	}

	// Take params for this tile
	DownloadRequestObject * request = pendingQueue.take(url);

	qDebug() << "Reply for: " << url;

	// Check if there is a redirect
	QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
	if (!redirect.isEmpty())
	{
		if (redirect.isRelative())
		{
			// Resolve full URL
			redirect = reply->url().resolved(redirect);
		}
		qDebug() << "Redirect: " << redirect.toString();

		// Add new request immediately
		// OR: Add to waiting queue with increased prio (in case some high prio job has come up)
		//     This would require changing the url of the request object
		pendingQueue.insert(redirect.toString(), request);
		networkManager->get(QNetworkRequest(redirect));
		++stats.numRedirects;

		// Schedule deletion of reply
		reply->deleteLater();

		return;
	}

	// Poll queue for new jobs
	pollQueue();

	// Forward response
	emit finished(request, reply);
	++stats.numProvides;
}

void DownloadManager::pollQueue()
{
	// Check for timeouts
	DownloadQueueIterator i(pendingQueue);
	while (i.hasNext())
	{
		i.next();
		int timePassed = QDateTime::currentMSecsSinceEpoch() - i.value()->startTime;
		if (timePassed > timeout)
		{
			// Abort job
			// Report as failed
		}
	}

	// Check if we can add another job
	if (pendingQueue.size() < maxConcurentJobs &&
		waitingQueue.size() > 0)
	{
		// Start next job
		DownloadRequestObject * request = nextJob();
		pendingQueue.insert(request->url, request);

		networkManager->get(QNetworkRequest(QUrl(request->url)));
		++stats.numRequests;

		qDebug() << "Requesting " << request->url;
	}
}

DownloadRequestObject * DownloadManager::nextJob()
{
	int highestPrio = INT_MAX;
	QString highestPrioKey;

	if (0 == waitingQueue.size())
		return NULL;

	// Look for the highest prio job
	DownloadQueueIterator i(waitingQueue);
	while (i.hasNext())
	{
		i.next();
		const int prio = i.value()->priority;
		if (prio < highestPrio)
		{
			highestPrio = prio;
			highestPrioKey = i.key();
		}
	}

	// Take job and return it
	return waitingQueue.take(highestPrioKey);
}
