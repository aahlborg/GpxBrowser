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
