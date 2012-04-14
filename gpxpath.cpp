#include "gpxpath.h"
#include <cfloat>

GPXPath::GPXPath()
{
}

void GPXPath::concatenate(const GPXPath &otherPath)
{
	const std::vector<GPXWaypoint> *otherWpts = otherPath.getWaypoints();
	for (unsigned int i = 0; i < otherWpts->size(); ++i)
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
		newPath.concatenate(waypoints[i]);
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

	for (unsigned int i = 0; i < waypoints.size(); ++i)
	{
		double distance = waypoints[i].distanceTo(otherWpt);
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

	for (unsigned int i = 0; i < waypoints.size() - 1; ++i)
	{
		length += waypoints[i].distanceTo(waypoints[i + 1]);
	}

	return length;
}

double GPXPath::duration() const
{
	// Time from first to last waypoint
	return waypoints[0].getTime().msecsTo(waypoints[waypoints.size() - 1].getTime()) / 1000.0;
}
