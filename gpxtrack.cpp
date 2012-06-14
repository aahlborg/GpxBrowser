#include "gpxtrack.h"
#include <QDomDocument>
#include <QDomElement>

GPXTrack::GPXTrack()
{
	// DEBUG DATA
	trackSegments_.append(GPXPath());
	// DEBUG DATA
}

void GPXTrack::outputXml(QDomDocument &document, QDomElement &trackElement) const
{
	// Add base elements
	GPXRouteBase::outputXml(document, trackElement);

	// Add track segments
	for (int i = 0; i < trackSegments_.size(); ++i)
	{
		QDomElement trksegElement = document.createElement("trkseg");
		trackSegments_.at(i).outputXml(document, trksegElement, "trkpt");
		trackElement.appendChild(trksegElement);
	}
}
