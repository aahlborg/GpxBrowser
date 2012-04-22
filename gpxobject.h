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
#include <vector>
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
	QString getVersion() const { return version; }
	QString getCreator() const { return creator; }
	GPXMetadata * getMetadata() { return &metadata; } // const?
	std::vector<GPXRoute> * getRoutes() { return &routes; } // const?
	std::vector<GPXTrack> * getTracks() { return &tracks; } // const?
	std::vector<GPXWaypoint> * getWaypoints() { return &waypoints; } // const?

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
	void setVersion(QString newVersion) { version = newVersion; }
	void setCreator(QString newCreator) { creator = newCreator; }

signals:
	
public slots:
	
private:
	QString version;
	QString creator;
	GPXMetadata metadata;
	std::vector<GPXRoute> routes;
	std::vector<GPXTrack> tracks;
	std::vector<GPXWaypoint> waypoints;
};

#endif // GPXOBJECT_H
