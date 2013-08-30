/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#include "gpxtrack.h"
#include <QDomDocument>
#include <QDomElement>

GPXTrack::GPXTrack()
{
	// DEBUG DATA
	//trackSegments_.append(GPXPath());
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

void GPXTrack::readXml(QDomElement &trackElement)
{
	GPXRouteBase::readXml(trackElement);

	QDomElement child = trackElement.firstChildElement();
	while (!child.isNull())
	{
		if (child.nodeName() == "trkseg")
		{
			qDebug() << "GPXTrack: Found track segment node";
			GPXPath trackSeg;
			trackSeg.readXml(child);
			trackSegments_.append(trackSeg);
		}

		child = child.nextSiblingElement();
	}
}

double GPXTrack::length() const
{
	double trackLength = 0.0;

	for (int i = 0; i < trackSegments_.size(); ++i)
	{
		trackLength += trackSegments_.at(i).length();
	}

	return trackLength;
}

double GPXTrack::duration() const
{
	double trackDuration = 0.0;

	for (int i = 0; i < trackSegments_.size(); ++i)
	{
		trackDuration += trackSegments_.at(i).duration();
	}

	return trackDuration;
}
