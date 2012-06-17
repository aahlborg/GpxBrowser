/** @class GPXRouteBase
 * Represents a common base for routes and tracks
 *
 * <...>
 *  <name>...</name>
 *  <cmt>...</cmt>
 *  <desc>...</desc>
 *  <link>...</link>
 *  ...
 * </...>
 */

#ifndef GPXROUTEBASE_H
#define GPXROUTEBASE_H

#include <QVector>
#include "gpxtypes.h"

class QDomDocument;
class QDomElement;

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname ## _ = false; \
	varname ## _ = defaultval; \
	}

class GPXRouteBase
{
public:
	GPXRouteBase();

	// Accessor methods for data members
	const QString getName() const { return name_; }
	const QString getComment() const { return comment_; }
	const QString getDescription() const { return description_; }
	const QVector<LinkType> * getLinks() { return &links_; }
	//int getNumber() const { return number; }

	// Functions informing which optional data are included
	bool hasName() const { return haveName_; }
	bool hasComment() const { return haveComment_; }
	bool hasDescription() const { return haveDescription_; }
	bool hasLinks() const { return haveLinks_; }
	//bool hasNumber() const { return haveNumber; }

	// Functions for clearing optional fields
	CLEAR_OPT(name, Name, "")
	CLEAR_OPT(comment, Comment, "")
	CLEAR_OPT(description, Description, "")
	void clearLinks() { links_.clear(); haveLinks_ = false; }
	//CLEAR_OPT(number, Number, 0)

	// Set optional fields
	void setName(const QString newName) { name_ = newName; haveName_ = true; }
	void setComment(const QString newComment) { comment_ = newComment; haveComment_ = true; }
	void setDescription(const QString newDescription) { description_ = newDescription; haveDescription_ = true; }
	void addLink(const LinkType newLink);
	void removeLink(const int linkIndex);
	//void setNumber(const int newNumber) { number = newNumber; haveNumber = true; }

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &routeBaseElement) const;
	void readXml(QDomElement &routeBaseElement);

private:
	QString name_;
	QString comment_;
	QString description_;
	QVector<LinkType> links_;
	//int number;

	bool haveName_;
	bool haveComment_;
	bool haveDescription_;
	bool haveLinks_;
	//bool haveNumber;
};

#endif // GPXROUTEBASE_H
