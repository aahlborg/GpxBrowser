#include "gpxmetadata.h"
#include "gpxutilities.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

GPXMetadata::GPXMetadata()
{
	clearName();
	clearDescription();
	clearAuthor();
	clearCopyright();
	clearLinks();
	clearTime();
	clearKeywords();
	clearBounds();

	// DEBUG DATA
	setName("Marcus Test");
	setDescription("Testfil for att testa saker.");

	PersonType author = getAuthor();
	author.name = "Marcus Ahlberg";
	author.hasName = true;

	author.email.id = "marcus";
	author.email.domain = "aahlborg.se";
	author.hasEmail = true;

	author.link.href = "http://www.topografix.com/gpx_validation.asp";
	author.link.text = "Make sure to validate output";
	author.link.hasText = true;
	author.hasLink = true;
	setAuthor(author);

	GPXDateTime newTime = GPXDateTime(QDate(2012, 03, 20), QTime(13, 37, 14, 430));
	setTime(newTime);

	BoundsType bounds = getBounds();
	bounds.minLat = 58.2;
	bounds.minLon = 11.5;
	bounds.maxLat = 58.6;
	bounds.maxLon = 11.8;
	setBounds(bounds);
	// DEBUG DATA
}

void GPXMetadata::addLink(const LinkType newLink)
{
	links_.append(newLink);
}

void GPXMetadata::removeLink(const int linkIndex)
{
	links_.erase(links_.begin() + linkIndex);

	if (links_.size() == 0)
		haveLinks_ = false;
}

bool GPXMetadata::setBounds(const BoundsType newBounds)
{
	if (GPXUtilities::validateBounds(newBounds))
	{
		bounds_ = newBounds;
		haveBounds_ = true;
		return true;
	}
	return false;
}

void GPXMetadata::outputXml(QDomDocument &document, QDomElement &metadataElement) const
{
	// Add name element
	if (haveName_)
	{
		QDomElement nameElement = document.createElement("name");
		QDomText nameElementText = document.createTextNode(name_);
		nameElement.appendChild(nameElementText);
		metadataElement.appendChild(nameElement);
	}

	// Add description element
	if (haveDescription_)
	{
		QDomElement descriptionElement = document.createElement("description");
		QDomText descriptionElementText = document.createTextNode(description_);
		descriptionElement.appendChild(descriptionElementText);
		metadataElement.appendChild(descriptionElement);
	}

	// Add author element
	if (haveAuthor_)
	{
		QDomElement authorElement = document.createElement("author");
		if (author_.hasName)
		{
			QDomElement nameElement = document.createElement("name");
			QDomText nameElementText = document.createTextNode(author_.name);
			nameElement.appendChild(nameElementText);
			authorElement.appendChild(nameElement);
		}
		if (author_.hasEmail)
		{
			QDomElement emailElement = document.createElement("email");
			QDomElement idElement = document.createElement("id");
			QDomText idElementText = document.createTextNode(author_.email.id);
			idElement.appendChild(idElementText);
			emailElement.appendChild(idElement);
			QDomElement domainElement = document.createElement("domain");
			QDomText domainElementText = document.createTextNode(author_.email.domain);
			domainElement.appendChild(domainElementText);
			emailElement.appendChild(domainElement);
			authorElement.appendChild(emailElement);
		}
		if (author_.hasLink)
		{
			QDomElement linkElement = document.createElement("link");
			linkElement.setAttribute("href", author_.link.href);
			if (author_.link.hasText)
			{
				QDomElement textElement = document.createElement("text");
				QDomText textElementText = document.createTextNode(author_.link.text);
				textElement.appendChild(textElementText);
				linkElement.appendChild(textElement);
			}
			if (author_.link.hasType)
			{
				QDomElement typeElement = document.createElement("type");
				QDomText typeElementText = document.createTextNode(author_.link.type);
				typeElement.appendChild(typeElementText);
				linkElement.appendChild(typeElement);
			}
			authorElement.appendChild(linkElement);
		}
		metadataElement.appendChild(authorElement);
	}

	// Add copyright element
	if (haveCopyright_)
	{
		QDomElement copyrightElement = document.createElement("copyright");
		copyrightElement.setAttribute("author", copyright_.author);
		if (copyright_.hasYear)
		{
			QDomElement yearElement = document.createElement("year");
			QDomText yearElementText = document.createTextNode(QString::number(copyright_.year));
			yearElement.appendChild(yearElementText);
			copyrightElement.appendChild(yearElement);
		}
		if (copyright_.hasLicense)
		{
			QDomElement licenseElement = document.createElement("license");
			QDomText licenseElementText = document.createTextNode(copyright_.license);
			licenseElement.appendChild(licenseElementText);
			copyrightElement.appendChild(licenseElement);
		}
		metadataElement.appendChild(copyrightElement);
	}

	// Add link elements
	if (haveLinks_)
	{
		for (int i = 0; i < links_.size(); ++i)
		{
			QDomElement linkElement = document.createElement("link");
			linkElement.setAttribute("href", links_[i].href);
			if (links_[i].hasText)
			{
				QDomElement textElement = document.createElement("text");
				QDomText textElementText = document.createTextNode(links_[i].text);
				textElement.appendChild(textElementText);
				linkElement.appendChild(textElement);
			}
			if (links_[i].hasType)
			{
				QDomElement typeElement = document.createElement("type");
				QDomText typeElementText = document.createTextNode(links_[i].type);
				typeElement.appendChild(typeElementText);
				linkElement.appendChild(typeElement);
			}
			metadataElement.appendChild(linkElement);
		}
	}

	// Add time element
	if (haveTime_)
	{
		QDomElement timeElement = document.createElement("time");
		QDomText timeElementText = document.createTextNode(time_.toString());
		timeElement.appendChild(timeElementText);
		metadataElement.appendChild(timeElement);
	}

	// Add keywords element
	if (haveKeywords_)
	{
		QDomElement keywordsElement = document.createElement("keywords");
		QDomText keywordsElementText = document.createTextNode(keywords_);
		keywordsElement.appendChild(keywordsElementText);
		metadataElement.appendChild(keywordsElement);
	}

	// Add bounds element
	if (haveBounds_)
	{
		QDomElement boundsElement = document.createElement("bounds");
		boundsElement.setAttribute("minlat", bounds_.minLat);
		boundsElement.setAttribute("minlon", bounds_.minLon);
		boundsElement.setAttribute("maxlat", bounds_.maxLat);
		boundsElement.setAttribute("maxlon", bounds_.maxLon);
		metadataElement.appendChild(boundsElement);
	}
}
