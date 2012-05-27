/** @class GPXMetadata
 * Holds metadata about a GPX object
 */

#ifndef GPXMETADATA_H
#define GPXMETADATA_H

#include <vector>
#include "gpxtypes.h"
#include "gpxdatetime.h"

class QDomDocument;
class QDomElement;

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname = false; \
	varname = defaultval; \
	}

class GPXMetadata
{
public:
	GPXMetadata();

	// Accessor methods for data members
	const QString getName() const { return name; }
	const QString getDescription() const { return description; }
	const PersonType getAuthor() const { return author; }
	const CopyrightType getCopyright() const { return copyright; }
	const std::vector<LinkType> * getLinks() const { return &links; }
	const GPXDateTime getTime() const { return time; }
	const QString getKeywords() const { return keywords; }
	const BoundsType getBounds() const { return bounds; }

	// Functions informing which optional data are included
	bool hasName() const { return haveName; }
	bool hasDescription() const { return haveDescription; }
	bool hasAuthor() const { return haveAuthor; }
	bool hasCopyright() const { return haveCopyright; }
	bool hasLinks() const { return haveLinks; }
	bool hasTime() const { return haveTime; }
	bool hasKeywords() const { return haveKeywords; }
	bool hasBounds() const { return haveBounds; }

	// Functions for clearing optional fields
	CLEAR_OPT(name, Name, "")
	CLEAR_OPT(description, Description, "")
	CLEAR_OPT(author, Author, EMPTY_PERSON_TYPE)
	CLEAR_OPT(copyright, Copyright, EMPTY_COPYRIGHT_TYPE)
	void clearLinks() { links.clear(); haveLinks = false; }
	CLEAR_OPT(time, Time, GPXDateTime())
	CLEAR_OPT(keywords, Keywords, "")
	CLEAR_OPT(bounds, Bounds, EMPTY_BOUNDS_TYPE)

	// Set optional fields
	void setName(QString newName) { name = newName; haveName = true; }
	void setDescription(const QString newDescription) { description = newDescription; haveDescription = true; }
	void setAuthor(const PersonType newAuthor) { author = newAuthor; haveAuthor = true; }
	void setCopyright(const CopyrightType newCopyright) { copyright = newCopyright; haveCopyright = true; }
	void addLink(const LinkType newLink);
	void removeLink(const int linkIndex);
	void setTime(const GPXDateTime newTime) { time = newTime; haveTime = true; }
	void setKeywords(const QString newKeywords) { keywords = newKeywords; haveKeywords = true; }
	bool setBounds(const BoundsType newBounds);

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &metadataElement) const;

private:
	// Optional data
	QString name;
	QString description;
	PersonType author;
	CopyrightType copyright;
	std::vector<LinkType> links;
	GPXDateTime time;
	QString keywords;
	BoundsType bounds;

	// Information about included optional data
	bool haveName;
	bool haveDescription;
	bool haveAuthor;
	bool haveCopyright;
	bool haveLinks;
	bool haveTime;
	bool haveKeywords;
	bool haveBounds;
};

#endif // GPXMETADATA_H
