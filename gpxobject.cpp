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
	waypoints_.append(GPXWaypoint());
	routes_.append(GPXRoute());
	tracks_.append(GPXTrack());
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
	gpx.setAttribute("xmlns", "http://www.topografix.com/GPX/1/1");
	gpx.setAttribute("xmlns:xalan", "http://xml.apache.org/xalan");
	gpx.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	gpx.setAttribute("version", version_);
	gpx.setAttribute("creator", creator_);
	doc.appendChild(gpx);

	// Metadata
	QDomElement metadataElement = doc.createElement("metadata");
	metadata_.outputXml(doc, metadataElement);
	gpx.appendChild(metadataElement);

	// Waypoints
	for (int i = 0; i < waypoints_.size(); ++i)
	{
		QDomElement wptElement = doc.createElement("wpt");
		waypoints_.at(i).outputXml(doc, wptElement);
		gpx.appendChild(wptElement);
	}

	// Routes
	for (int i = 0; i < routes_.size(); ++i)
	{
		QDomElement rteElement = doc.createElement("rte");
		routes_.at(i).outputXml(doc, rteElement);
		gpx.appendChild(rteElement);
	}

	// Tracks
	for (int i = 0; i < tracks_.size(); ++i)
	{
		QDomElement trkElement = doc.createElement("trk");
		tracks_.at(i).outputXml(doc, trkElement);
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
