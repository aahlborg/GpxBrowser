/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

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
#include <QVector>
#include "gpxpath.h"

class QDomDocument;
class QDomElement;

class GPXTrack : public GPXRouteBase
{
public:
	GPXTrack();

	const QVector<GPXPath> * getSegments() const { return &trackSegments_; }

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &trackElement) const;
	void readXml(QDomElement &trackElement);

	// Calculations methods
	double length() const;
	double duration() const;

private:
	QVector<GPXPath> trackSegments_;
};

#endif // GPXTRACK_H
