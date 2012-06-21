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

void GPXRoute::readXml(QDomElement &routeElement)
{
	GPXRouteBase::readXml(routeElement);

	qDebug() << "GPXRoute: Reading self as path";
	path_.readXml(routeElement);
}
