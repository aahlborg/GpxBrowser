/** @class GPXDateTime
 * Handles date and time in UTC and provides methods
 * for printing GPX format date and time
 */

#ifndef GPXDATETIME_H
#define GPXDATETIME_H

#include <QDateTime>

class GPXDateTime : public QDateTime
{
public:
	explicit GPXDateTime();
	explicit GPXDateTime(const QDate &date);
	explicit GPXDateTime(const QDate &date, const QTime &time);
	explicit GPXDateTime(const QDateTime &dateTime);

	QString toString() const;
	static GPXDateTime fromString(const QString &s);
};

#endif // GPXDATETIME_H
