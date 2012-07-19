#include "gpxdatetime.h"

GPXDateTime::GPXDateTime() :
	QDateTime()
{
	setTimeSpec(Qt::UTC);
}

GPXDateTime::GPXDateTime(const QDate &date) :
	QDateTime()
{
	setTimeSpec(Qt::UTC);
	setDate(date);
	setTime(QTime(0, 0, 0, 0));
}

GPXDateTime::GPXDateTime(const QDate &date, const QTime &time) :
	QDateTime()
{
	setTimeSpec(Qt::UTC);
	setDate(date);
	setTime(time);
}

GPXDateTime::GPXDateTime(const QDateTime &dateTime) :
	QDateTime()
{
	setTimeSpec(Qt::UTC);
	setDate(dateTime.toUTC().date());
	setTime(dateTime.toUTC().time());
}

QString GPXDateTime::toString() const
{
	if (0 == time().msec())
		return QDateTime::toString("yyyy-MM-ddThh:mm:ssZ");
	else
		return QDateTime::toString("yyyy-MM-ddThh:mm:ss.zzzZ");
}

// TODO: Verify that there aren't any problems with local time/utc here
GPXDateTime GPXDateTime::fromString(const QString &s)
{
	QDateTime qDateTime = QDateTime::fromString(s, Qt::ISODate);
	GPXDateTime gpxDateTime = GPXDateTime(qDateTime.date(), qDateTime.time());
	return gpxDateTime;
}
