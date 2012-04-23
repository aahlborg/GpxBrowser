#ifndef GPXTYPES_H
#define GPXTYPES_H

#include <QtCore>
#include <vector>

typedef double Latitude;
typedef double Longitude;


/** @struct LinkType
 * Represents a link to an external resource
 *
 * <link href="...">
 *  <text>...</text>
 *  <type>...</type>
 * </link>
 */
struct LinkType
{
	QString href;
	QString text;
	QString type;
	bool hasText;
	bool hasType;
};

const LinkType EMPTY_LINK_TYPE = {"", "", "", false, false};

/** @struct EmailType
 * Represents an email address, id@domain
 *
 * <email>
 *  <id>...</id>
 *  <domain>...</domain>
 * </email>
 */
struct EmailType
{
	QString id;
	QString domain;
};

const EmailType EMPTY_EMAIL_TYPE = {"", ""};

/** @struct CopyrightType
 * Represents copyright information
 *
 * <copyright author="...">
 *  <year>...</year>
 *  <license>...</license>
 * </copyright>
 */
struct CopyrightType
{
	QString author;
	int year;
	QString license;
	bool hasYear;
	bool hasLicense;
};

const CopyrightType EMPTY_COPYRIGHT_TYPE  = {"", 0, "", false, false};

/** @struct PersonType
 * Represents a person
 *
 * <person>
 *  <name>...</name>
 *  <link> [LinkType] </link>
 *  <email> [EmailType] </email>
 * </person>
 */
struct PersonType
{
	QString name;
	LinkType link;
	EmailType email;
	bool hasName;
	bool hasEmail;
	bool hasLink;
};

const PersonType EMPTY_PERSON_TYPE  = {"", EMPTY_LINK_TYPE, EMPTY_EMAIL_TYPE, false, false, false};

/** @struct BoundsType
 * Represents the outer bounds of a gpx object
 *
 * <bounds
 *  minlat="..."
 *  minlon="..."
 *  maxlat="..."
 *  maxlon="..." />
 */
struct BoundsType
{
	Latitude minLat;
	Longitude minLon;
	Latitude maxLat;
	Longitude maxLon;
};

const BoundsType EMPTY_BOUNDS_TYPE = {0.0, 0.0, 0.0, 0.0};

#endif // GPXTYPES_H
