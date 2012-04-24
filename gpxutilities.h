#ifndef GPXUTILITIES_H
#define GPXUTILITIES_H

#include "gpxtypes.h"

class GPXUtilities
{
public:
	// Calculation methods
	static double distanceTo(const double lat1, const double lon1, const double lat2, const double lon2);
	static double headingTo(const double lat1, const double lon1, const double lat2, const double lon2);

	// Conversion methods
	static double toRad(const double degrees);
	static double toDeg(const double radians);

	// Utility methods
	static double sqr(const double x);
	static bool validateLatitude(const double lat);
	static bool validateLongitude(const double lon);
	static bool validatePosition(const double lat, const double lon);
	static bool validateBounds(const BoundsType bounds);

private:
	// Private settings
	static bool useEllipsoid; // Whether to use ellipsoid or spherical Earth in calculations

	// Private constants
	static const double pi; // Blueberry flavored favorite constant
	static const double earthMeanSphereRadius; // Minimizes root mean square of distance calculations

	// Private methods
	static double distanceTo_sphere(const double lat1, const double lon1, const double lat2, const double lon2);
	static double distanceTo_ellipsoid(const double lat1, const double lon1, const double lat2, const double lon2);
	static double headingTo_sphere(const double lat1, const double lon1, const double lat2, const double lon2);
	static double headingTo_ellipsoid(const double lat1, const double lon1, const double lat2, const double lon2);
};

//
// Inline function definitions
//

inline double GPXUtilities::toRad(const double degrees)
{
	return degrees * pi / 180;
}

inline double GPXUtilities::toDeg(const double radians)
{
	return radians * 180 / pi;
}

inline double GPXUtilities::sqr(const double x)
{
	return x * x;
}

inline bool GPXUtilities::validateLatitude(const double lat)
{
	return (lat <= 90.0) && (lat >= -90.0);
}

inline bool GPXUtilities::validateLongitude(const double lon)
{
	return (lon <= 180.0) && (lon >= -180.0);
}

inline bool GPXUtilities::validatePosition(const double lat, const double lon)
{
	return validateLatitude(lat) && validateLongitude(lon);
}

inline bool GPXUtilities::validateBounds(const BoundsType bounds)
{
	return validateLatitude(bounds.minLat) &&
		   validateLatitude(bounds.maxLat) &&
		   validateLongitude(bounds.minLon) &&
		   validateLongitude(bounds.maxLon);
}

#endif // GPXUTILITIES_H
