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

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &routeElement) const;

private:
	GPXPath path;
};

#endif // GPXROUTE_H
