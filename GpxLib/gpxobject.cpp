#include "gpxobject.h"
#include "gpxtypes.h"
#include <QDomDocument>
#include <QDebug>

GPXObject::GPXObject(QObject *parent) :
	QObject(parent)
{
	setVersion(GPX_1_1);
	setCreator("GPXlib Alpha - https://bitbucket.org/aahlborg/gpxbrowser");

	// DEBUG DATA
	//waypoints_.append(GPXWaypoint());
	//routes_.append(GPXRoute());
	//tracks_.append(GPXTrack());
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
	gpx.setAttribute("version", getVersionString());
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

// DEBUG
void printAttributes(QDomNode &node)
{
	qDebug() << "GPXObject: " << node.nodeName() << " attributes:";
	for (int i = 0; i < node.attributes().count(); ++i)
	{
		qDebug() << "GPXObject: " << node.attributes().item(i).nodeName() << ": " << node.attributes().item(i).nodeValue();
	}
}
void printChildren(QDomNode &node)
{
	qDebug() << "GPXObject: " << node.nodeName() << " children:";
	for (int i = 0; i < node.childNodes().count(); ++i)
	{
		qDebug() << "GPXObject: " << node.childNodes().item(i).nodeName() << ": " << node.childNodes().item(i).nodeValue();
	}
}
// DEBUG

GPXObject * GPXObject::loadFromFile(QIODevice * file)
{
	QDomDocument doc;
	QString errStr;
	int errLine;
	int errCol;

	// Read and parse XML file
	if (!doc.setContent(file, &errStr, &errLine, &errCol))
	{
		qDebug() << QString("GPXObject: Error: Parse error at line %1, column %2:\n%3").arg(errLine).arg(errCol).arg(errStr);
		return NULL;
	}

	// Process root element, <gpx>
	QDomElement gpx = doc.documentElement();
	qDebug() << "GPXObject: NodeName: " << gpx.nodeName();
	if (gpx.nodeName() != "gpx")
	{
		qDebug() << "GPXObject: Error: Invalid node name " << gpx.nodeName();
		return NULL;
	}

	if (!gpx.hasAttribute("version"))
	{
		qDebug() << "GPXObject: Error: Missing version attribute";
		return NULL;
	}

	GpxVersion version = getVersionFromString(gpx.attribute("version"));
	if (GPX_INVALID_VERSION == version)
	{
		qDebug() << "GPXObject: Error: Invalid GPX version";
		return NULL;
	}

	printAttributes(gpx);
	printChildren(gpx);

	// Create GPXObject
	GPXObject * gpxObj = new GPXObject();
	gpxObj->setVersion(version);

	// Look through child nodes
	for (int i = 0; i < gpx.childNodes().count(); ++i)
	{
		QDomElement child = gpx.childNodes().item(i).toElement();

		if (child.isNull())
		{
			qDebug() << "GPXObject: Warning: Node is not an element " << child.nodeName();
		}

		if (child.nodeName() == "trk")
		{
			qDebug() << "GPXObject: Found track node";
			GPXTrack track;
			track.readXml(child);
			gpxObj->getTracks()->append(track);
		}
		else if (child.nodeName() == "rte")
		{
			qDebug() << "GPXObject: Found route node";
			GPXRoute route;
			route.readXml(child);
			gpxObj->getRoutes()->append(route);
		}
		else if (child.nodeName() == "wpt")
		{
			qDebug() << "GPXObject: Found wpt node";
			GPXWaypoint wpt;
			wpt.readXml(child);
			gpxObj->getWaypoints()->append(wpt);
		}
		else
		{
			qDebug() << "GPXObject: Warning: Skipping element " << child.nodeName();
		}
	}

	qDebug() << "GPXObject: Done";

	return gpxObj;
}

void GPXObject::exportKML(const QIODevice * /*file*/) const
{
}

GPXObject * GPXObject::importKML(const QIODevice * /*file*/)
{
	return NULL;
}

void GPXObject::exportCSV(const QIODevice * /*file*/) const
{
}

GpxVersion GPXObject::getVersionFromString(QString versionStr)
{
	if (GPX_1_0_STR == versionStr)
	{
		return GPX_1_0;
	}
	else if (GPX_1_1_STR == versionStr)
	{
		return GPX_1_1;
	}
	else
	{
		qDebug() << "GPXObject: Error: Invalid GPX version string";
		return GPX_INVALID_VERSION;
	}
}

QString GPXObject::getVersionString() const
{
	if (GPX_1_0 == version_)
	{
		return GPX_1_0_STR;
	}
	else if (GPX_1_1 == version_)
	{
		return GPX_1_1_STR;
	}
	else
	{
		qDebug() << "GPXObject: Error: Invalid GPX version";
		return QString();
	}
}
