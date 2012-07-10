#include "gpxutilities.h"
#include <cmath>

// Private constants
const double GPXUtilities::pi = 3.14159265358979;
const double GPXUtilities::earthMeanSphereRadius = 6372800.0;
const double GPXUtilities::wgs84EquatorialRadius = 6378137.0;
const double GPXUtilities::wgs84ReciprocalFlattening = 298.257223563;

double GPXUtilities::distanceTo(const double lat1, const double lon1, const double lat2, const double lon2)
{
	return distanceTo_lambert(lat1, lon1, lat2, lon2);
}

double GPXUtilities::headingTo(const double lat1, const double lon1, const double lat2, const double lon2)
{
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

double GPXUtilities::distanceTo_lambert(const double lat1, const double lon1, const double lat2, const double lon2)
{
	if (lat1 == lat2 && lon1 == lon2)
		return 0.0;

	// Lambert's formulae
	const double r = wgs84ReciprocalFlattening; // Reciprocal of the flattening of the WGS84 ellipsoid
	const double a = wgs84EquatorialRadius; // Equatorial radius of Earth
	// Reduced latitudes
	const double redLat1 = atan(((r - 1) / r) * tan(lat1));
	const double redLat2 = atan(((r - 1) / r) * tan(lat2));
	// Calculate central angle
	const double A = sqr(sin((redLat2 - redLat1) / 2)) + cos(redLat1) * cos(redLat2) * sqr(sin((lon2 - lon1) / 2));
	const double sigma = 2 * atan2(sqrt(A), sqrt(1 - A));
	// Calculations
	const double P = (redLat1 + redLat2) / 2;
	const double Q = (redLat2 - redLat1) / 2;
	const double X = (sigma - sin(sigma)) * sqr(sin(P)) * sqr(cos(Q)) / sqr(cos(sigma / 2));
	const double Y = (sigma + sin(sigma)) * sqr(cos(P)) * sqr(sin(Q)) / sqr(sin(sigma / 2));
	// Distance
	const double s = a * (sigma - (X + Y) / (2 * r));
	return s;
}

double GPXUtilities::headingTo_sphere(const double lat1, const double lon1, const double lat2, const double lon2)
{
	const double dLon = lon2 - lon1;

	return atan2(sin(dLon) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon));
}
