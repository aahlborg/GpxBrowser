#include "gpxutilities.h"
#include <cmath>

bool GPXUtilities::useEllipsoid = false;

// Private constants
const double GPXUtilities::pi = 3.14159265358979;
const double GPXUtilities::earthMeanSphereRadius = 6372800;

double GPXUtilities::distanceTo(const double lat1, const double lon1, const double lat2, const double lon2)
{
	if (useEllipsoid)
		return distanceTo_ellipsoid(lat1, lon1, lat2, lon2);
	else
		return distanceTo_sphere(lat1, lon1, lat2, lon2);
}

double GPXUtilities::headingTo(const double lat1, const double lon1, const double lat2, const double lon2)
{
	if (useEllipsoid)
		return headingTo_ellipsoid(lat1, lon1, lat2, lon2);
	else
		return headingTo_sphere(lat1, lon1, lat2, lon2);
}

// Calculating distance between two coordinates using an appriximated spherical Earth
double GPXUtilities::distanceTo_sphere(const double lat1, const double lon1, const double lat2, const double lon2)
{
	const double dLat = (lat2 - lat1);
	const double dLon = (lon2 - lon1);
	const double a = sqr(sin(dLat / 2)) + cos(lat1) * cos(lat2) * sqr(sin(dLon / 2));
	const double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return c * earthMeanSphereRadius;
}

double GPXUtilities::distanceTo_ellipsoid(const double /*lat1*/, const double /*lon1*/, const double /*lat2*/, const double /*lon2*/)
{
	return -1.0;
}

double GPXUtilities::headingTo_sphere(const double lat1, const double lon1, const double lat2, const double lon2)
{
	const double dLon = lon2 - lon1;

	return atan2(sin(dLon) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon));
}

double GPXUtilities::headingTo_ellipsoid(const double /*lat1*/, const double /*lon1*/, const double /*lat2*/, const double /*lon2*/)
{
	return -1.0;
}
