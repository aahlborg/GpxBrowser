/** @class GPXWaypoint
 * Represents a GPX waypoint
 *
 * <wpt lat="..." lon="...">
 *  <ele>...</ele>
 *  <time>...</time>
 *  <name>...</name>
 *  <cmt>...</cmt>
 *  <desc>...</desc>
 *  <link>...</link>
 * </wpt>
 */

#ifndef GPXWAYPOINT_H
#define GPXWAYPOINT_H

#include <vector>
#include "gpxtypes.h"
#include "gpxdatetime.h"

class QDomDocument;
class QDomElement;

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname = false; \
	varname = defaultval; \
	}

class GPXWaypoint
{
public:
	GPXWaypoint();
	explicit GPXWaypoint(const double newLatitude, const double newLongitude);

	// Accessor methods for data members
	double getLatitude() const { return latitude; }
	double getLongitude() const { return longitude; }
	double getElevation() const { return elevation; }
	const GPXDateTime getTime() const { return time; }
	const QString getName() const { return name; }
	const QString getComment() const { return comment; }
	const QString getDescription() const { return description; }
	const std::vector<LinkType> * getLinks() { return &links; }

	// Functions informing which optional data are included
	bool hasElevation() const { return haveElevation; }
	bool hasTime() const { return haveTime; }
	bool hasName() const { return haveName; }
	bool hasComment() const { return haveComment; }
	bool hasDescription() const { return haveDescription; }
	bool hasLinks() const { return haveLinks; }

	// Functions for clearing optional fields
	CLEAR_OPT(elevation, Elevation, 0.0)
	CLEAR_OPT(time, Time, GPXDateTime())
	CLEAR_OPT(name, Name, "")
	CLEAR_OPT(comment, Comment, "")
	CLEAR_OPT(description, Description, "")
	void clearLinks() { links.clear(); haveLinks = false; }

	// Set position
	bool setPosition(const double newLatitude, const double newLongitude);
	bool setLatitude(const double newLatitude);
	bool setLongitude(const double newLongitude);

	// Set optional fields
	void setElevation(const double newElevation) { elevation = newElevation; haveElevation = true; }
	void setTime(const GPXDateTime newTime) { time = newTime; haveTime = true; }
	void setName(const QString newName) { name = newName; haveName = true; }
	void setComment(const QString newComment) { comment = newComment; haveComment = true; }
	void setDescription(const QString newDescription) { description = newDescription; haveDescription = true; }
	void addLink(const LinkType newLink);
	void removeLink(const int linkIndex);

	// Data manipulation methods

	// Calculations methods
	double distanceTo(const GPXWaypoint &otherWpt) const;
	double headingTo(const GPXWaypoint &otherWpt) const;
	double speedTo(const GPXWaypoint &otherWpt) const;

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &wptElement) const;

private:
	// Position data
	double latitude;
	double longitude;

	// Optional data
	double elevation;
	GPXDateTime time;
	QString name;
	QString comment;
	QString description;
	std::vector<LinkType> links;

	// Information about included optional data
	bool haveElevation;
	bool haveTime;
	bool haveName;
	bool haveComment;
	bool haveDescription;
	bool haveLinks;
};

#endif // GPXWAYPOINT_H
