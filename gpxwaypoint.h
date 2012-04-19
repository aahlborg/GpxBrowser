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

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname = false; \
	varname = defaultval; \
	}

class GPXWaypoint
{
public:
	GPXWaypoint();

	// Accessor methods for data members
	const float getLatitude() const { return latitude; }
	const float getLongitude() const { return longitude; }
	const float getElevation() const { return elevation; }
	const GPXDateTime getTime() const { return time; }
	const QString getName() const { return name; }
	const QString getComment() const { return comment; }
	const QString getDescription() const { return description; }
	const std::vector<LinkType> * const getLinks() { return &links; }

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
	void setPosition(const float newLatitude, const float newLongitude) { latitude = newLatitude; longitude = newLongitude; }
	void setLatitude(const float newLatitude) { latitude = newLatitude; }
	void setLongitude(const float newLongitude) { longitude = newLongitude; }

	// Set optional fields
	void setElevation(const float newElevation) { elevation = newElevation; haveElevation = true; }
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

	// XML import/export methods
	// getXML();

private:
	// Position data
	float latitude;
	float longitude;

	// Optional data
	float elevation;
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
