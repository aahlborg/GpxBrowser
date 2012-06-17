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

#include <QVector>
#include "gpxtypes.h"
#include "gpxdatetime.h"

class QDomDocument;
class QDomElement;

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname ## _ = false; \
	varname ## _ = defaultval; \
	}

class GPXWaypoint
{
public:
	GPXWaypoint();
	explicit GPXWaypoint(const double newLatitude, const double newLongitude);

	// Accessor methods for data members
	double getLatitude() const { return latitude_; }
	double getLongitude() const { return longitude_; }
	double getElevation() const { return elevation_; }
	const GPXDateTime getTime() const { return time_; }
	const QString getName() const { return name_; }
	const QString getComment() const { return comment_; }
	const QString getDescription() const { return description_; }
	const QVector<LinkType> * getLinks() { return &links_; }

	// Functions informing which optional data are included
	bool hasElevation() const { return haveElevation_; }
	bool hasTime() const { return haveTime_; }
	bool hasName() const { return haveName_; }
	bool hasComment() const { return haveComment_; }
	bool hasDescription() const { return haveDescription_; }
	bool hasLinks() const { return haveLinks_; }

	// Functions for clearing optional fields
	CLEAR_OPT(elevation, Elevation, 0.0)
	CLEAR_OPT(time, Time, GPXDateTime())
	CLEAR_OPT(name, Name, "")
	CLEAR_OPT(comment, Comment, "")
	CLEAR_OPT(description, Description, "")
	void clearLinks() { links_.clear(); haveLinks_ = false; }

	// Set position
	bool setPosition(const double newLatitude, const double newLongitude);
	bool setLatitude(const double newLatitude);
	bool setLongitude(const double newLongitude);

	// Set optional fields
	void setElevation(const double newElevation) { elevation_ = newElevation; haveElevation_ = true; }
	void setTime(const GPXDateTime newTime) { time_ = newTime; haveTime_ = true; }
	void setName(const QString newName) { name_ = newName; haveName_ = true; }
	void setComment(const QString newComment) { comment_ = newComment; haveComment_ = true; }
	void setDescription(const QString newDescription) { description_ = newDescription; haveDescription_ = true; }
	void addLink(const LinkType newLink);
	void removeLink(const int linkIndex);

	// Data manipulation methods

	// Calculations methods
	double distanceTo(const GPXWaypoint &otherWpt) const;
	double headingTo(const GPXWaypoint &otherWpt) const;
	double speedTo(const GPXWaypoint &otherWpt) const;

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &wptElement) const;
	void readXml(QDomElement &wptElement);

private:
	// Position data
	double latitude_;
	double longitude_;

	// Optional data
	double elevation_;
	GPXDateTime time_;
	QString name_;
	QString comment_;
	QString description_;
	QVector<LinkType> links_;

	// Information about included optional data
	bool haveElevation_;
	bool haveTime_;
	bool haveName_;
	bool haveComment_;
	bool haveDescription_;
	bool haveLinks_;
};

#endif // GPXWAYPOINT_H
