#ifndef GPXUTILITIES_H
#define GPXUTILITIES_H

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

private:
	// Private settings
	static bool useEllipsoid; // Whether to use ellipsoid or spherical Earth in calculations

	// Private constants
	static const double pi;// = 3.14159265358979; // Blueberry flavored favorite constant
	static const double earthMeanSphereRadius;// = 6372800; // Minimizes root mean square of distance calculations

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

#endif // GPXUTILITIES_H
