/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

/** @class GPXObject
 * Represents a complete GPX file
 *
 * <gpx version="1.1" creator="...">
 *  <metadata>...</metadata>
 *  <wpt>...</wpt>
 *  <rte>...</rte>
 *  <trk>...</trk>
 * </gpx>
 */

#ifndef GPXOBJECT_H
#define GPXOBJECT_H

#include <QObject>
#include <QVector>
#include "gpxtypes.h"
#include "gpxroute.h"
#include "gpxtrack.h"
#include "gpxwaypoint.h"
#include "gpxmetadata.h"

// GPX versions
enum GpxVersion
{
	GPX_INVALID_VERSION,
	GPX_1_0,
	GPX_1_1
};

// GPX version strings
#define GPX_1_0_STR "1.0"
#define GPX_1_1_STR "1.1"

class GPXObject : public QObject
{
	Q_OBJECT
public:
	explicit GPXObject(QObject *parent = 0);

	// Accessor methods
	GpxVersion getVersion() const { return version_; }
	QString getCreator() const { return creator_; }
	GPXMetadata * getMetadata() { return &metadata_; } // const?
	QVector<GPXRoute> * getRoutes() { return &routes_; } // const?
	QVector<GPXTrack> * getTracks() { return &tracks_; } // const?
	QVector<GPXWaypoint> * getWaypoints() { return &waypoints_; } // const?

	// Load/save methods
	void saveToFile(QIODevice *file) const;
	static GPXObject * loadFromFile(QIODevice *file);

	// Import/export other formats
	void exportKML(const QIODevice *file) const;
	static GPXObject * importKML(const QIODevice * file);
	void exportCSV(const QIODevice *file) const;

protected:
	// These fields are related to the syntax of the saved GPX file
	// and should not be modifiable by the user
	void setVersion(GpxVersion newVersion) { version_ = newVersion; }
	void setCreator(QString newCreator) { creator_ = newCreator; }

signals:
	
public slots:
	
private:
	static GpxVersion getVersionFromString(QString versionStr);

	QString getVersionString() const;

	GpxVersion version_;
	QString creator_;
	GPXMetadata metadata_;
	QVector<GPXRoute> routes_;
	QVector<GPXTrack> tracks_;
	QVector<GPXWaypoint> waypoints_;
};

#endif // GPXOBJECT_H
