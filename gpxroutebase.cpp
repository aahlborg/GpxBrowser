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
	links_.append(newLink);
	haveLinks_ = true;
}

void GPXRouteBase::removeLink(const int linkIndex)
{
	links_.erase(links_.begin() + linkIndex);

	if (links_.size() == 0)
		haveLinks_ = false;
}

void GPXRouteBase::outputXml(QDomDocument &document, QDomElement &routeBaseElement) const
{
	// Add name element
	if (haveName_)
	{
		QDomElement nameElement = document.createElement("name");
		QDomText nameElementText = document.createTextNode(name_);
		nameElement.appendChild(nameElementText);
		routeBaseElement.appendChild(nameElement);
	}

	// Add cmt element
	if (haveComment_)
	{
		QDomElement cmtElement = document.createElement("cmt");
		QDomText cmtElementText = document.createTextNode(comment_);
		cmtElement.appendChild(cmtElementText);
		routeBaseElement.appendChild(cmtElement);
	}

	// Add desc element
	if (haveDescription_)
	{
		QDomElement descElement = document.createElement("desc");
		QDomText descElementText = document.createTextNode(description_);
		descElement.appendChild(descElementText);
		routeBaseElement.appendChild(descElement);
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
			routeBaseElement.appendChild(linkElement);
		}
	}
}
