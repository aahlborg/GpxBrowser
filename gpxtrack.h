/** @class GPXTrack
 * Represents a GPX track
 *
 * <trk>
 *  <name>...</name>
 *  <cmt>...</cmt>
 *  <desc>...</desc>
 *  <link>...</link>
 * (<number>...</number>)
 *  <trkseg>
 *   <trkpt>...</trkpt>
 *  </trkseg>
 * </trk>
 */

#ifndef GPXTRACK_H
#define GPXTRACK_H

#include "gpxroutebase.h"
#include <vector>
#include "gpxpath.h"

class QDomDocument;
class QDomElement;

class GPXTrack : public GPXRouteBase
{
public:
	GPXTrack();

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &trackElement) const;

private:
	std::vector<GPXPath> trackSegments;
};

#endif // GPXTRACK_H
