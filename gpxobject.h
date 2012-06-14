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

class GPXObject : public QObject
{
	Q_OBJECT
public:
	explicit GPXObject(QObject *parent = 0);

	// Accessor methods
	QString getVersion() const { return version_; }
	QString getCreator() const { return creator_; }
	GPXMetadata * getMetadata() { return &metadata_; } // const?
	QVector<GPXRoute> * getRoutes() { return &routes_; } // const?
	QVector<GPXTrack> * getTracks() { return &tracks_; } // const?
	QVector<GPXWaypoint> * getWaypoints() { return &waypoints_; } // const?

	// Load/save methods
	void saveToFile(QIODevice *file) const;
	static GPXObject * loadFromFile(const QString fileName);

	// Import/export other formats
	void exportKML(const QString fileName) const;
	static GPXObject * importKML(QString fileName);
	void exportCSV(const QString fileName) const;

protected:
	// These fields are related to the syntax of the saved GPX file
	// and should not be modifiable by the user
	void setVersion(QString newVersion) { version_ = newVersion; }
	void setCreator(QString newCreator) { creator_ = newCreator; }

signals:
	
public slots:
	
private:
	QString version_;
	QString creator_;
	GPXMetadata metadata_;
	QVector<GPXRoute> routes_;
	QVector<GPXTrack> tracks_;
	QVector<GPXWaypoint> waypoints_;
};

#endif // GPXOBJECT_H
