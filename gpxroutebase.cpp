#include "gpxroutebase.h"
#include <QDomDocument>
#include <QDomElement>

GPXRouteBase::GPXRouteBase()
{
	// DEBUG DATA
	setName("RouteBase alpha");
	setComment("First post!");
	setDescription("Base for routes and tracks");
	LinkType link = EMPTY_LINK_TYPE;
	link.href = "http://openstreetmap.org";
	addLink(link);
	// DEBUG DATA
}

void GPXRouteBase::addLink(const LinkType newLink)
{
	links.push_back(newLink);
	haveLinks = true;
}

void GPXRouteBase::removeLink(const int linkIndex)
{
	links.erase(links.begin() + linkIndex);

	if (links.size() == 0)
		haveLinks = false;
}

void GPXRouteBase::outputXml(QDomDocument &document, QDomElement &routeBaseElement) const
{
	// Add name element
	if (haveName)
	{
		QDomElement nameElement = document.createElement("name");
		QDomText nameElementText = document.createTextNode(name);
		nameElement.appendChild(nameElementText);
		routeBaseElement.appendChild(nameElement);
	}

	// Add cmt element
	if (haveComment)
	{
		QDomElement cmtElement = document.createElement("cmt");
		QDomText cmtElementText = document.createTextNode(comment);
		cmtElement.appendChild(cmtElementText);
		routeBaseElement.appendChild(cmtElement);
	}

	// Add desc element
	if (haveDescription)
	{
		QDomElement descElement = document.createElement("desc");
		QDomText descElementText = document.createTextNode(description);
		descElement.appendChild(descElementText);
		routeBaseElement.appendChild(descElement);
	}

	// Add link elements
	if (haveLinks)
	{
		for (uint i = 0; i < links.size(); ++i)
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
			routeBaseElement.appendChild(linkElement);
		}
	}
}
