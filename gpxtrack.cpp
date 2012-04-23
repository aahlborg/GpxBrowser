#include "gpxtrack.h"
#include <QDomDocument>
#include <QDomElement>

GPXTrack::GPXTrack()
{
	// DEBUG DATA
	trackSegments.push_back(GPXPath());
	// DEBUG DATA
}

void GPXTrack::outputXml(QDomDocument &document, QDomElement &trackElement) const
{
	// Add base elements
	GPXRouteBase::outputXml(document, trackElement);

	// Add track segments
	for (uint i = 0; i < trackSegments.size(); ++i)
	{
		QDomElement trksegElement = document.createElement("trkseg");
		trackSegments.at(i).outputXml(document, trksegElement, "trkpt");
		trackElement.appendChild(trksegElement);
	}
}
