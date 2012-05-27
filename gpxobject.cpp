#include "gpxobject.h"
#include "gpxtypes.h"
#include <QDomDocument>
#include <QDebug>

GPXObject::GPXObject(QObject *parent) :
	QObject(parent)
{
	setVersion("1.1");
	setCreator("GPXlib Alpha - https://bitbucket.org/aahlborg/gpxbrowser");

	// DEBUG DATA
	waypoints.push_back(GPXWaypoint());
	routes.push_back(GPXRoute());
	tracks.push_back(GPXTrack());
	// DEBUG DATA
}

void GPXObject::saveToFile(QIODevice * file) const
{
	// Create document
	QDomDocument doc;
	QDomProcessingInstruction pi = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" ");
	doc.insertBefore(pi, QDomNode());

	// Root <gpx> node
	QDomElement gpx = doc.createElement("gpx");
	gpx.setAttribute("version", version);
	gpx.setAttribute("creator", creator);
	doc.appendChild(gpx);

	// Metadata
	QDomElement metadataElement = doc.createElement("metadata");
	metadata.outputXml(doc, metadataElement);
	gpx.appendChild(metadataElement);

	// Waypoints
	for (uint i = 0; i < waypoints.size(); ++i)
	{
		QDomElement wptElement = doc.createElement("wpt");
		waypoints.at(i).outputXml(doc, wptElement);
		gpx.appendChild(wptElement);
	}

	// Routes
	for (uint i = 0; i < routes.size(); ++i)
	{
		QDomElement rteElement = doc.createElement("rte");
		routes.at(i).outputXml(doc, rteElement);
		gpx.appendChild(rteElement);
	}

	// Tracks
	for (uint i = 0; i < tracks.size(); ++i)
	{
		QDomElement trkElement = doc.createElement("trk");
		tracks.at(i).outputXml(doc, trkElement);
		gpx.appendChild(trkElement);
	}

	// Save to file
	const int indentSize = 2;
	QTextStream fileStream(file);
	fileStream.setCodec("UTF-8");
	doc.save(fileStream, indentSize);

	qDebug() << "Saved file";
}

GPXObject * GPXObject::loadFromFile(const QString /*fileName*/)
{
	return NULL;
}

void GPXObject::exportKML(const QString /*fileName*/) const
{
}

GPXObject * GPXObject::importKML(QString /*fileName*/)
{
	return NULL;
}

void GPXObject::exportCSV(const QString /*fileName*/) const
{
}
