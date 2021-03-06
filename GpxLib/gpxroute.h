/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

/** @class GPXRoute
 * Represents a GPX route
 *
 * <rte>
 *  <name>...</name>
 *  <cmt>...</cmt>
 *  <desc>...</desc>
 *  <link>...</link>
 * (<number>...</number>)
 *  <rtept>...</rtept>
 * </rte>
 */

#ifndef GPXROUTE_H
#define GPXROUTE_H

#include "gpxroutebase.h"
#include "gpxpath.h"

class QDomDocument;
class QDomElement;

class GPXRoute : public GPXRouteBase
{
public:
	GPXRoute();

	GPXPath & getPath() { return path_; }

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &routeElement) const;
	void readXml(QDomElement &routeElement);

	// Calculations methods
	double length() const { return path_.length(); }
	double duration() const { return path_.duration(); }

private:
	GPXPath path_;
};

#endif // GPXROUTE_H
