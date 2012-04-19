#include "gpxmetadata.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

GPXMetadata::GPXMetadata()
{
	haveName = false;
	haveDescription = false;
	haveAuthor = false;
	haveCopyright = false;
	haveLinks = false;
	haveTime = false;
	haveKeywords = false;
	haveBounds = false;

	setName("Marcus Test");
	qDebug() << "Metadata.name " << getName();
	setDescription("Testfil för att testa saker.");

	PersonType author = getAuthor();
	author.name = "Marcus Ahlberg";
	author.hasName = true;

	author.email.id = "marcus";
	author.email.domain = "aahlborg.se";
	author.hasEmail = true;

	author.link.href = "http://gpxbrowser.aahlborg.se";
	author.link.text = "GPXBrowser homepage";
	author.link.hasText = true;
	author.hasLink = true;
	setAuthor(author);

	GPXDateTime newTime = GPXDateTime(QDate(2012, 03, 20), QTime(13, 37, 14));
	setTime(newTime);

	BoundsType bounds = getBounds();
	bounds.minLat = 58.2f;
	bounds.minLon = 11.5f;
	bounds.maxLat = 58.6f;
	bounds.maxLon = 11.8f;
	setBounds(bounds);
}

void GPXMetadata::addLink(const LinkType newLink)
{
	links.push_back(newLink);
}

void GPXMetadata::removeLink(const int linkIndex)
{
	links.erase(links.begin() + linkIndex);

	if (links.size() == 0)
		haveLinks = false;
}

void GPXMetadata::outputXml(QDomDocument &document, QDomElement &metadataElement) const
{
	// Add name element
	if (haveName)
	{
		QDomElement nameElement = document.createElement("name");
		QDomText nameElementText = document.createTextNode(name);
		nameElement.appendChild(nameElementText);
		metadataElement.appendChild(nameElement);
	}

	// Add description element
	if (haveDescription)
	{
		QDomElement descriptionElement = document.createElement("description");
		QDomText descriptionElementText = document.createTextNode(description);
		descriptionElement.appendChild(descriptionElementText);
		metadataElement.appendChild(descriptionElement);
	}

	// Add author element
	if (haveAuthor)
	{
		QDomElement authorElement = document.createElement("author");
		if (author.hasName)
		{
			QDomElement nameElement = document.createElement("name");
			QDomText nameElementText = document.createTextNode(author.name);
			nameElement.appendChild(nameElementText);
			authorElement.appendChild(nameElement);
		}
		if (author.hasEmail)
		{
			QDomElement emailElement = document.createElement("email");
			QDomElement idElement = document.createElement("id");
			QDomText idElementText = document.createTextNode(author.email.id);
			idElement.appendChild(idElementText);
			emailElement.appendChild(idElement);
			QDomElement domainElement = document.createElement("domain");
			QDomText domainElementText = document.createTextNode(author.email.domain);
			domainElement.appendChild(domainElementText);
			emailElement.appendChild(domainElement);
			authorElement.appendChild(emailElement);
		}
		if (author.hasLink)
		{
			QDomElement linkElement = document.createElement("link");
			linkElement.setAttribute("href", author.link.href);
			if (author.link.hasText)
			{
				QDomElement textElement = document.createElement("text");
				QDomText textElementText = document.createTextNode(author.link.text);
				textElement.appendChild(textElementText);
				linkElement.appendChild(textElement);
			}
			if (author.link.hasType)
			{
				QDomElement typeElement = document.createElement("type");
				QDomText typeElementText = document.createTextNode(author.link.type);
				typeElement.appendChild(typeElementText);
				linkElement.appendChild(typeElement);
			}
			authorElement.appendChild(linkElement);
		}
		metadataElement.appendChild(authorElement);
	}

	// Add copyright element
	if (haveCopyright)
	{
		QDomElement copyrightElement = document.createElement("copyright");
		copyrightElement.setAttribute("author", copyright.author);
		if (copyright.hasYear)
		{
			QDomElement yearElement = document.createElement("year");
			QDomText yearElementText = document.createTextNode(QString::number(copyright.year));
			yearElement.appendChild(yearElementText);
			copyrightElement.appendChild(yearElement);
		}
		if (copyright.hasLicense)
		{
			QDomElement licenseElement = document.createElement("license");
			QDomText licenseElementText = document.createTextNode(copyright.license);
			licenseElement.appendChild(licenseElementText);
			copyrightElement.appendChild(licenseElement);
		}
		metadataElement.appendChild(copyrightElement);
	}

	// Add link elements
	if (haveLinks)
	{
		for (unsigned int i = 0; i < links.size(); ++i)
		{
			QDomElement linkElement = document.createElement("link");
			linkElement.setAttribute("href", links[i].href);
			if (links[i].hasText)
			{
				QDomElement textElement = document.createElement("text");
				QDomText textElementText = document.createTextNode(links[i].text);
				textElement.appendChild(textElementText);
				linkElement.appendChild(textElement);
			}
			if (links[i].hasType)
			{
				QDomElement typeElement = document.createElement("type");
				QDomText typeElementText = document.createTextNode(links[i].type);
				typeElement.appendChild(typeElementText);
				linkElement.appendChild(typeElement);
			}
			metadataElement.appendChild(linkElement);
		}
	}

	// Add time element
	if (haveTime)
	{
		QDomElement timeElement = document.createElement("time");
		QDomText timeElementText = document.createTextNode(time.toString());
		timeElement.appendChild(timeElementText);
		metadataElement.appendChild(timeElement);
	}

	// Add keywords element
	if (haveKeywords)
	{
		QDomElement keywordsElement = document.createElement("keywords");
		QDomText keywordsElementText = document.createTextNode(keywords);
		keywordsElement.appendChild(keywordsElementText);
		metadataElement.appendChild(keywordsElement);
	}

	// Add bounds element
	if (haveBounds)
	{
		QDomElement boundsElement = document.createElement("bounds");
		boundsElement.setAttribute("minlat", bounds.minLat);
		boundsElement.setAttribute("minlon", bounds.minLon);
		boundsElement.setAttribute("maxlat", bounds.maxLat);
		boundsElement.setAttribute("maxlon", bounds.maxLon);
		metadataElement.appendChild(boundsElement);
	}
}
