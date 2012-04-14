/** @class GPXPath
 * Represents a series of connected waypoints in a route or a track
 */

#ifndef GPXPATH_H
#define GPXPATH_H

#include <vector>
#include "gpxwaypoint.h"

class GPXPath
{
public:
	GPXPath();

	// Accessor methods for data members
	const std::vector<GPXWaypoint> *getWaypoints() const { return &waypoints; }

	// Modification methods
	void concatenate(const GPXPath &otherPath);
	void concatenate(const GPXWaypoint &newWpt);

	// Extraction methods
	GPXPath subPath(const int start, const int end) const;
	GPXWaypoint closestWaypoint(const GPXWaypoint &otherWpt) const;
	int closestWaypointIndex(const GPXWaypoint &otherWpt) const;

	// Calculations methods
	double length() const;
	double duration() const;

private:
	std::vector<GPXWaypoint> waypoints;
};

#endif // GPXPATH_H
