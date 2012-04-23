#include "gpxpath.h"
#include <cfloat>
#include <QDomDocument>
#include <QDomElement>

GPXPath::GPXPath()
{
	// DEBUG DATA
	concatenate(GPXWaypoint(57.720829072, 11.941928406));
	concatenate(GPXWaypoint(57.720877325, 11.942108352));
	concatenate(GPXWaypoint(57.720890608, 11.942267466));
	// DEBUG DATA
}

void GPXPath::concatenate(const GPXPath &otherPath)
{
	const std::vector<GPXWaypoint> *otherWpts = otherPath.getWaypoints();
	for (uint i = 0; i < otherWpts->size(); ++i)
	{
		this->concatenate(otherWpts->at(i));
	}
}

void GPXPath::concatenate(const GPXWaypoint &newWpt)
{
	waypoints.push_back(newWpt); // Maybe want to copy newWpt?
}

GPXPath GPXPath::subPath(const int start, const int end) const
{
	GPXPath newPath;

	// From start up to and including end
	for (int i = start; i <= end; ++i)
	{
		newPath.concatenate(waypoints.at(i));
	}

	return newPath;
}

GPXWaypoint GPXPath::closestWaypoint(const GPXWaypoint &otherWpt) const
{
	return waypoints[closestWaypointIndex(otherWpt)];
}

int GPXPath::closestWaypointIndex(const GPXWaypoint &otherWpt) const
{
	double shortestDistance = DBL_MAX;
	int iClosest = -1;

	for (uint i = 0; i < waypoints.size(); ++i)
	{
		double distance = waypoints.at(i).distanceTo(otherWpt);
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

	for (uint i = 0; i < waypoints.size() - 1; ++i)
	{
		length += waypoints.at(i).distanceTo(waypoints[i + 1]);
	}

	return length;
}

double GPXPath::duration() const
{
	// Time from first to last waypoint
	return waypoints.at(0).getTime().msecsTo(waypoints.at(waypoints.size() - 1).getTime()) / 1000.0;
}

void GPXPath::outputXml(QDomDocument &document, QDomElement &pathElement, const QString nodeName) const
{
	for (uint i = 0; i < waypoints.size(); ++i)
	{
		QDomElement wptElement = document.createElement(nodeName);
		waypoints.at(i).outputXml(document, wptElement);
		pathElement.appendChild(wptElement);
	}
}
