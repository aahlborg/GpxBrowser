#include "gpxobject.h"
#include "gpxtypes.h"
#include <QDomDocument>
#include <QDebug>

GPXObject::GPXObject(QObject *parent) :
	QObject(parent)
{
	version = "1.1";
	creator = "GPXlib Alpha - http://gpxbrowser.aahlborg.se";
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

	// Routes

	// Tracks

	// Save to file
	const int indentSize = 2;
	QTextStream fileStream(file);
	fileStream.setCodec("UTF-8");
	doc.save(fileStream, indentSize);

	qDebug() << doc.documentElement().nodeName();

	qDebug() << "Saved file";
}

GPXObject * GPXObject::loadFromFile(const QString fileName)
{
	return NULL;
}

void GPXObject::exportKML(const QString fileName) const
{
}

GPXObject * GPXObject::importKML(QString fileName)
{
	return NULL;
}

void GPXObject::exportCSV(const QString fileName) const
{
}
