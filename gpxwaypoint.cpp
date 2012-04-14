#include "gpxwaypoint.h"
#include "gpxutilities.h"

GPXWaypoint::GPXWaypoint()
{
}

void GPXWaypoint::addLink(const LinkType newLink)
{
	links.push_back(newLink);
}

void GPXWaypoint::removeLink(const int linkIndex)
{
	links.erase(links.begin() + linkIndex);

	if (links.size() == 0)
		haveLinks = false;
}

double GPXWaypoint::distanceTo(const GPXWaypoint &otherWpt) const
{
	double lat1 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon1 = GPXUtilities::toRad(otherWpt.getLongitude());
	double lat2 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon2 = GPXUtilities::toRad(otherWpt.getLongitude());

	return GPXUtilities::distanceTo(lat1, lon1, lat2, lon2);
}

double GPXWaypoint::headingTo(const GPXWaypoint &otherWpt) const
{
	double lat1 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon1 = GPXUtilities::toRad(otherWpt.getLongitude());
	double lat2 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon2 = GPXUtilities::toRad(otherWpt.getLongitude());

	return GPXUtilities::toDeg(GPXUtilities::headingTo(lat1, lon1, lat2, lon2));
}

double GPXWaypoint::speedTo(const GPXWaypoint &otherWpt) const
{
	double distance = distanceTo(otherWpt);
	qint64 msecs = this->getTime().msecsTo(otherWpt.getTime());

	return distance / (msecs / 1000.0);
}
