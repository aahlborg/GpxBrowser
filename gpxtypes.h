#ifndef GPXTYPES_H
#define GPXTYPES_H

#include <QtCore>
#include <vector>

typedef float Latitude;
typedef float Longitude;


/** @struct LinkType
 * Represents a link to an external resource
 */
struct LinkType
{
	QString href;
	QString name;
	QString mimeType;
};

/** @struct EmailType
 * Represents an email address
 */
struct EmailType
{
	QString id;
	QString domain;
};

/** @struct CopyrightType
 * Represents copyright information
 */
struct CopyrightType
{
	QString author;
	int year;
	QString license;
};

/** @struct PersonType
 * Represents a person
 */
struct PersonType
{
	QString name;
	LinkType link;
	EmailType email;
};

/** @struct BoundsType
 * Represents the outer bounds of a track
 */
struct BoundsType
{
	Latitude minLat;
	Longitude minLon;
	Latitude maxLat;
	Longitude maxLon;
};

/** @struct MetadataType
 * Holds metadata about a GPX object
 */
struct MetadataType
{
	QString name;
	QString desc;
	PersonType author;
	CopyrightType copyright;
	std::vector<LinkType> links;
	QDateTime time;
	QString keywords;
	BoundsType bounds;
};

#endif // GPXTYPES_H
