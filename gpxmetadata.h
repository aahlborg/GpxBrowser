/** @class GPXMetadata
 * Holds metadata about a GPX object
 */

#ifndef GPXMETADATA_H
#define GPXMETADATA_H

#include <QVector>
#include "gpxtypes.h"
#include "gpxdatetime.h"

class QDomDocument;
class QDomElement;

#define CLEAR_OPT(varname, camelname, defaultval) void clear ## camelname() { \
	have ## camelname ## _ = false; \
	varname ## _ = defaultval; \
	}

class GPXMetadata
{
public:
	GPXMetadata();

	// Accessor methods for data members
	const QString getName() const { return name_; }
	const QString getDescription() const { return description_; }
	const PersonType getAuthor() const { return author_; }
	const CopyrightType getCopyright() const { return copyright_; }
	const QVector<LinkType> * getLinks() const { return &links_; }
	const GPXDateTime getTime() const { return time_; }
	const QString getKeywords() const { return keywords_; }
	const BoundsType getBounds() const { return bounds_; }

	// Functions informing which optional data are included
	bool hasName() const { return haveName_; }
	bool hasDescription() const { return haveDescription_; }
	bool hasAuthor() const { return haveAuthor_; }
	bool hasCopyright() const { return haveCopyright_; }
	bool hasLinks() const { return haveLinks_; }
	bool hasTime() const { return haveTime_; }
	bool hasKeywords() const { return haveKeywords_; }
	bool hasBounds() const { return haveBounds_; }

	// Functions for clearing optional fields
	CLEAR_OPT(name, Name, "")
	CLEAR_OPT(description, Description, "")
	CLEAR_OPT(author, Author, EMPTY_PERSON_TYPE)
	CLEAR_OPT(copyright, Copyright, EMPTY_COPYRIGHT_TYPE)
	void clearLinks() { links_.clear(); haveLinks_ = false; }
	CLEAR_OPT(time, Time, GPXDateTime())
	CLEAR_OPT(keywords, Keywords, "")
	CLEAR_OPT(bounds, Bounds, EMPTY_BOUNDS_TYPE)

	// Set optional fields
	void setName(QString newName) { name_ = newName; haveName_ = true; }
	void setDescription(const QString newDescription) { description_ = newDescription; haveDescription_ = true; }
	void setAuthor(const PersonType newAuthor) { author_ = newAuthor; haveAuthor_ = true; }
	void setCopyright(const CopyrightType newCopyright) { copyright_ = newCopyright; haveCopyright_ = true; }
	void addLink(const LinkType newLink);
	void removeLink(const int linkIndex);
	void setTime(const GPXDateTime newTime) { time_ = newTime; haveTime_ = true; }
	void setKeywords(const QString newKeywords) { keywords_ = newKeywords; haveKeywords_ = true; }
	bool setBounds(const BoundsType newBounds);

	// GPX XML output
	void outputXml(QDomDocument &document, QDomElement &metadataElement) const;

private:
	// Optional data
	QString name_;
	QString description_;
	PersonType author_;
	CopyrightType copyright_;
	QVector<LinkType> links_;
	GPXDateTime time_;
	QString keywords_;
	BoundsType bounds_;

	// Information about included optional data
	bool haveName_;
	bool haveDescription_;
	bool haveAuthor_;
	bool haveCopyright_;
	bool haveLinks_;
	bool haveTime_;
	bool haveKeywords_;
	bool haveBounds_;
};

#endif // GPXMETADATA_H
