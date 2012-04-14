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

class GPXObject : public QObject
{
	Q_OBJECT
public:
	explicit GPXObject(QObject *parent = 0);

	// Load/save methods
	void saveToFile(const QString fileName) const;
	static GPXObject * loadFromFile(const QString fileName);

	// Import/export other formats
	void exportKML(const QString fileName) const;
	static GPXObject * importKML(QString fileName);
	void exportCSV(const QString fileName) const;

signals:
	
public slots:
	
private:
	QString version;
	QString creator;
	MetadataType metadata;
	std::vector<GPXRoute> routes;
	std::vector<GPXTrack> tracks;
	std::vector<GPXWaypoint> waypoints;
};

#endif // GPXOBJECT_H
