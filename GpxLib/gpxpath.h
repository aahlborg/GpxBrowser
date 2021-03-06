/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

/** @class GPXPath
 * Represents a series of connected waypoints in a route or a track
 *
 * <...>
 *  <...pt>...</...pt>
 * </...>
 */

#ifndef GPXPATH_H
#define GPXPATH_H

#include <QVector>
#include "gpxwaypoint.h"

class QDomDocument;
class QDomElement;

class GPXPath
{
public:
	GPXPath();

	// Accessor methods for data members
	const QVector<GPXWaypoint> * getWaypoints() const { return &waypoints_; }

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

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &pathElement, const QString nodeName) const;
	void readXml(QDomElement &pathElement);

private:
	QVector<GPXWaypoint> waypoints_;
};

#endif // GPXPATH_H
