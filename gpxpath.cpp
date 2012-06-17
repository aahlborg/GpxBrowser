#include "gpxpath.h"
#include <cfloat>
#include <QDomDocument>
#include <QDomElement>

GPXPath::GPXPath()
{
	// DEBUG DATA
	//concatenate(GPXWaypoint(57.720829072, 11.941928406));
	//concatenate(GPXWaypoint(57.720877325, 11.942108352));
	//concatenate(GPXWaypoint(57.720890608, 11.942267466));
	// DEBUG DATA
}

void GPXPath::concatenate(const GPXPath &otherPath)
{
	const QVector<GPXWaypoint> *otherWpts = otherPath.getWaypoints();
	for (int i = 0; i < otherWpts->size(); ++i)
	{
		this->concatenate(otherWpts->at(i));
	}
}

void GPXPath::concatenate(const GPXWaypoint &newWpt)
{
	waypoints_.append(newWpt); // Maybe want to copy newWpt?
}

GPXPath GPXPath::subPath(const int start, const int end) const
{
	GPXPath newPath;

	// From start up to and including end
	for (int i = start; i <= end; ++i)
	{
		newPath.concatenate(waypoints_.at(i));
	}

	return newPath;
}

GPXWaypoint GPXPath::closestWaypoint(const GPXWaypoint &otherWpt) const
{
	return waypoints_[closestWaypointIndex(otherWpt)];
}

int GPXPath::closestWaypointIndex(const GPXWaypoint &otherWpt) const
{
	double shortestDistance = DBL_MAX;
	int iClosest = -1;

	for (int i = 0; i < waypoints_.size(); ++i)
	{
		double distance = waypoints_.at(i).distanceTo(otherWpt);
		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			iClosest = i;
		}
	}

	return iClosest;
}

double GPXPath::length() const
{
	double length = 0;

	for (int i = 0; i < waypoints_.size() - 1; ++i)
	{
		length += waypoints_.at(i).distanceTo(waypoints_[i + 1]);
	}

	return length;
}

double GPXPath::duration() const
{
	// Time from first to last waypoint
	return waypoints_.at(0).getTime().msecsTo(waypoints_.at(waypoints_.size() - 1).getTime()) / 1000.0;
}

void GPXPath::outputXml(QDomDocument &document, QDomElement &pathElement, const QString nodeName) const
{
	for (int i = 0; i < waypoints_.size(); ++i)
	{
		QDomElement wptElement = document.createElement(nodeName);
		waypoints_.at(i).outputXml(document, wptElement);
		pathElement.appendChild(wptElement);
	}
}

void GPXPath::readXml(QDomElement &pathElement)
{
	for (int i = 0; i < pathElement.childNodes().count(); ++i)
	{
		QDomElement child = pathElement.childNodes().item(i).toElement();
		if (child.nodeName() == "trkpt" ||
			child.nodeName() == "rtept")
		{
			//qDebug() << "GPXPath: Found waypoint node";
			GPXWaypoint wpt;
			wpt.readXml(child);
			waypoints_.append(wpt);
		}
	}
}
