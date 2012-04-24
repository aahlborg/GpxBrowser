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

#include <vector>
#include "gpxtypes.h"

class QDomDocument;
class QDomElement;

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname = false; \
	varname = defaultval; \
	}

class GPXRouteBase
{
public:
	GPXRouteBase();

	// Accessor methods for data members
	const QString getName() const { return name; }
	const QString getComment() const { return comment; }
	const QString getDescription() const { return description; }
	const std::vector<LinkType> * getLinks() { return &links; }
	//int getNumber() const { return number; }

	// Functions informing which optional data are included
	bool hasName() const { return haveName; }
	bool hasComment() const { return haveComment; }
	bool hasDescription() const { return haveDescription; }
	bool hasLinks() const { return haveLinks; }
	//bool hasNumber() const { return haveNumber; }

	// Functions for clearing optional fields
	CLEAR_OPT(name, Name, "")
	CLEAR_OPT(comment, Comment, "")
	CLEAR_OPT(description, Description, "")
	void clearLinks() { links.clear(); haveLinks = false; }
	//CLEAR_OPT(number, Number, 0)

	// Set optional fields
	void setName(const QString newName) { name = newName; haveName = true; }
	void setComment(const QString newComment) { comment = newComment; haveComment = true; }
	void setDescription(const QString newDescription) { description = newDescription; haveDescription = true; }
	void addLink(const LinkType newLink);
	void removeLink(const int linkIndex);
	//void setNumber(const int newNumber) { number = newNumber; haveNumber = true; }

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &routeBaseElement) const;

private:
	QString name;
	QString comment;
	QString description;
	std::vector<LinkType> links;
	//int number;

	bool haveName;
	bool haveComment;
	bool haveDescription;
	bool haveLinks;
	//bool haveNumber;
};

#endif // GPXROUTEBASE_H
