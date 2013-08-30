/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#include "downloadmanager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>

// Debug
#include <QDebug>

DownloadManager::DownloadManager(QObject *parent) :
	QObject(parent),
	maxConcurentJobs_(8),
	timeout_(10000),
	userAgent_("GpxBrowser 0.1 (Dev Alpha) gpxbrowser@aahlborg.se")
{
	networkManager_ = new QNetworkAccessManager();
	connect(networkManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReply(QNetworkReply*)));
}

DownloadManager::~DownloadManager()
{
	delete networkManager_;
}

void DownloadManager::request(DownloadRequestObject * request)
{
	waitingQueue_.insert(request->url, request);
	pollQueue();
}

void DownloadManager::prioritize(int delta)
{
	// Change priority for all items in queue
	//for (int i = 0; i < waitingQueue.size(); ++i)
	DownloadQueueIterator i(waitingQueue_);
	while (i.hasNext())
	{
		//waitingQueue.at(i).priority += delta;
		i.next();
		i.value()->priority += delta;
	}
}

void DownloadManager::setMaxConcurentJobs(int numConnections)
{
	maxConcurentJobs_ = numConnections;
}

void DownloadManager::networkReply(QNetworkReply * reply)
{
	QString url = reply->url().toString();

	stats_.numDownloadedBytes += reply->bytesAvailable();

	// Check for errors
	if (reply->error())
	{
		qDebug() << "DownloadManager: Network error: " << reply->error();
		pendingQueue_.remove(url);
		reply->deleteLater();
		++stats_.numFailedRequests;
		return;
	}
	else if (!pendingQueue_.contains(url))
	{
		qDebug() << "DownloadManager: Error: No pending request for " << url;
		reply->deleteLater();
		++stats_.numFailedRequests;
		return;
	}

	// Take params for this tile
	DownloadRequestObject * request = pendingQueue_.take(url);

	qDebug() << "DownloadManager: Reply for: " << url;

	// Check if there is a redirect
	QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
	if (!redirect.isEmpty())
	{
		if (redirect.isRelative())
		{
			// Resolve full URL
			redirect = reply->url().resolved(redirect);
		}
		qDebug() << "DownloadManager: Redirect: " << redirect.toString();

		// Add new request immediately
		// OR: Add to waiting queue with increased prio (in case some high prio job has come up)
		//     This would require changing the url of the request object
		pendingQueue_.insert(redirect.toString(), request);
		QNetworkRequest networkRequest(redirect);
		networkRequest.setRawHeader("User-Agent", userAgent_);
		networkManager_->get(networkRequest);
		++stats_.numRedirects;

		// Schedule deletion of reply
		reply->deleteLater();

		return;
	}

	// Poll queue for new jobs
	pollQueue();

	// Forward response
	emit finished(request, reply);
	++stats_.numProvides;
}

void DownloadManager::pollQueue()
{
	// Check for timeouts
	DownloadQueueIterator i(pendingQueue_);
	while (i.hasNext())
	{
		i.next();
		int timePassed = QDateTime::currentMSecsSinceEpoch() - i.value()->startTime;
		if (timePassed > timeout_)
		{
			// Abort job
			// Report as failed
		}
	}

	// Check if we can add another job
	if (pendingQueue_.size() < maxConcurentJobs_ &&
		waitingQueue_.size() > 0)
	{
		// Start next job
		DownloadRequestObject * request = nextJob();
		pendingQueue_.insert(request->url, request);

		QNetworkRequest networkRequest(QUrl(request->url));
		networkRequest.setRawHeader("User-Agent", userAgent_);
		networkManager_->get(networkRequest);
		++stats_.numRequests;

		qDebug() << "DownloadManager: Requesting " << request->url;
	}
}

DownloadRequestObject * DownloadManager::nextJob()
{
	int highestPrio = INT_MAX;
	QString highestPrioKey;

	if (0 == waitingQueue_.size())
		return NULL;

	// Look for the highest prio job
	DownloadQueueIterator i(waitingQueue_);
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
	return waitingQueue_.take(highestPrioKey);
}
