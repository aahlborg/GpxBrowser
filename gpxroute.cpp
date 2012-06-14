#include "gpxroute.h"

GPXRoute::GPXRoute()
{
}

void GPXRoute::outputXml(QDomDocument &document, QDomElement &routeElement) const
{
	// Add base elements
	GPXRouteBase::outputXml(document, routeElement);

	// Add route path
	path_.outputXml(document, routeElement, "rtept");
}
