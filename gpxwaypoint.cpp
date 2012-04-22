#include "gpxwaypoint.h"
#include "gpxutilities.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

GPXWaypoint::GPXWaypoint()
{
	setPosition(0.0, 0.0);
	clearElevation();
	clearTime();
	clearName();
	clearComment();
	clearDescription();
	clearLinks();

	// DEBUG DATA
	setPosition(58.34824, 18.23421);
	setElevation(23);
	setTime(GPXDateTime::fromString("2012-04-22T21:23:23Z"));
	setName("Testpunkt");
	setComment("Don't trust this point");
	setDescription("Made up position");
	LinkType link = EMPTY_LINK_TYPE;
	link.href = "http://www.google.com";
	link.text = "Search and thou shall find";
	link.hasText = true;
	addLink(link);
	// DEBUG DATA
}

GPXWaypoint::GPXWaypoint(const float newLatitude, const float newLongitude)
{
	setPosition(newLatitude, newLongitude);
	clearElevation();
	clearTime();
	clearName();
	clearComment();
	clearDescription();
	clearLinks();
}

void GPXWaypoint::addLink(const LinkType newLink)
{
	links.push_back(newLink);
	haveLinks = true;
}

void GPXWaypoint::removeLink(const int linkIndex)
{
	links.erase(links.begin() + linkIndex);

	if (links.size() == 0)
		haveLinks = false;
}

double GPXWaypoint::distanceTo(const GPXWaypoint &otherWpt) const
{
	double lat1 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon1 = GPXUtilities::toRad(otherWpt.getLongitude());
	double lat2 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon2 = GPXUtilities::toRad(otherWpt.getLongitude());

	return GPXUtilities::distanceTo(lat1, lon1, lat2, lon2);
}

double GPXWaypoint::headingTo(const GPXWaypoint &otherWpt) const
{
	double lat1 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon1 = GPXUtilities::toRad(otherWpt.getLongitude());
	double lat2 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon2 = GPXUtilities::toRad(otherWpt.getLongitude());

	return GPXUtilities::toDeg(GPXUtilities::headingTo(lat1, lon1, lat2, lon2));
}

double GPXWaypoint::speedTo(const GPXWaypoint &otherWpt) const
{
	double distance = distanceTo(otherWpt);
	qint64 msecs = this->getTime().msecsTo(otherWpt.getTime());

	return distance / (msecs / 1000.0);
}

void GPXWaypoint::outputXml(QDomDocument &document, QDomElement &wptElement) const
{
	// Add mandatory position attributes
	wptElement.setAttribute("lat", latitude);
	wptElement.setAttribute("lon", longitude);

	// Add elevation element
	if (haveElevation)
	{
		QDomElement eleElement = document.createElement("ele");
		QDomText eleElementText = document.createTextNode(QString::number(elevation));
		eleElement.appendChild(eleElementText);
		wptElement.appendChild(eleElement);
	}

	// Add time element
	if (haveTime)
	{
		QDomElement timeElement = document.createElement("time");
		QDomText timeElementText = document.createTextNode(time.toString());
		timeElement.appendChild(timeElementText);
		wptElement.appendChild(timeElement);
	}

	// Add name element
	if (haveName)
	{
		QDomElement nameElement = document.createElement("name");
		QDomText nameElementText = document.createTextNode(name);
		nameElement.appendChild(nameElementText);
		wptElement.appendChild(nameElement);
	}

	// Add cmt element
	if (haveComment)
	{
		QDomElement cmtElement = document.createElement("cmt");
		QDomText cmtElementText = document.createTextNode(comment);
		cmtElement.appendChild(cmtElementText);
		wptElement.appendChild(cmtElement);
	}

	// Add desc element
	if (haveDescription)
	{
		QDomElement descElement = document.createElement("desc");
		QDomText descElementText = document.createTextNode(description);
		descElement.appendChild(descElementText);
		wptElement.appendChild(descElement);
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
			wptElement.appendChild(linkElement);
		}
	}
}
