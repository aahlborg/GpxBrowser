/* Copyright (c) 2013 Marcus Ahlberg
 *
 * This file is part of GPX Browser and distributed under
 * the Simplified BSD License. For full terms, see the
 * included LICENSE file.
 */

#include "gpxwaypoint.h"
#include "gpxutilities.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

GPXWaypoint::GPXWaypoint()
{
	clearData();

	// DEBUG DATA
	/*setPosition(58.34824, 18.23421);
	setElevation(23);
	setTime(GPXDateTime::fromString("2012-04-22T21:23:23Z"));
	setName("Testpunkt");
	setComment("Don't trust this point");
	setDescription("Made up position");
	LinkType link = EMPTY_LINK_TYPE;
	link.href = "http://www.google.com";
	link.text = "Search and thou shall find";
	link.hasText = true;
	addLink(link);*/
	// DEBUG DATA
}

GPXWaypoint::GPXWaypoint(const double newLatitude, const double newLongitude)
{
	clearData();
	setPosition(newLatitude, newLongitude);
}

void GPXWaypoint::clearData()
{
	setPosition(0.0, 0.0);
	clearElevation();
	clearTime();
	clearName();
	clearComment();
	clearDescription();
	clearLinks();
}

bool GPXWaypoint::setPosition(const double newLatitude, const double newLongitude)
{
	if (GPXUtilities::validatePosition(newLatitude, newLongitude))
	{
		latitude_ = newLatitude;
		longitude_ = newLongitude;
		return true;
	}
	return false;
}

bool GPXWaypoint::setLatitude(const double newLatitude)
{
	if (GPXUtilities::validateLatitude(newLatitude))
	{
		latitude_ = newLatitude;
		return true;
	}
	return false;
}

bool GPXWaypoint::setLongitude(const double newLongitude)
{
	if (GPXUtilities::validateLongitude(newLongitude))
	{
		longitude_ = newLongitude;
		return true;
	}
	return false;
}

void GPXWaypoint::addLink(const LinkType newLink)
{
	links_.append(newLink);
	haveLinks_ = true;
}

void GPXWaypoint::removeLink(const int linkIndex)
{
	links_.erase(links_.begin() + linkIndex);

	if (links_.size() == 0)
		haveLinks_ = false;
}

double GPXWaypoint::distanceTo(const GPXWaypoint &otherWpt) const
{
	double lat1 = GPXUtilities::toRad(getLatitude());
	double lon1 = GPXUtilities::toRad(getLongitude());
	double lat2 = GPXUtilities::toRad(otherWpt.getLatitude());
	double lon2 = GPXUtilities::toRad(otherWpt.getLongitude());

	return GPXUtilities::distanceTo(lat1, lon1, lat2, lon2);
}

double GPXWaypoint::headingTo(const GPXWaypoint &otherWpt) const
{
	double lat1 = GPXUtilities::toRad(getLatitude());
	double lon1 = GPXUtilities::toRad(getLongitude());
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
	wptElement.setAttribute("lat", latitude_);
	wptElement.setAttribute("lon", longitude_);

	// Add elevation element
	if (haveElevation_)
	{
		QDomElement eleElement = document.createElement("ele");
		QDomText eleElementText = document.createTextNode(QString::number(elevation_));
		eleElement.appendChild(eleElementText);
		wptElement.appendChild(eleElement);
	}

	// Add time element
	if (haveTime_)
	{
		QDomElement timeElement = document.createElement("time");
		QDomText timeElementText = document.createTextNode(time_.toString());
		timeElement.appendChild(timeElementText);
		wptElement.appendChild(timeElement);
	}

	// Add name element
	if (haveName_)
	{
		QDomElement nameElement = document.createElement("name");
		QDomText nameElementText = document.createTextNode(name_);
		nameElement.appendChild(nameElementText);
		wptElement.appendChild(nameElement);
	}

	// Add cmt element
	if (haveComment_)
	{
		QDomElement cmtElement = document.createElement("cmt");
		QDomText cmtElementText = document.createTextNode(comment_);
		cmtElement.appendChild(cmtElementText);
		wptElement.appendChild(cmtElement);
	}

	// Add desc element
	if (haveDescription_)
	{
		QDomElement descElement = document.createElement("desc");
		QDomText descElementText = document.createTextNode(description_);
		descElement.appendChild(descElementText);
		wptElement.appendChild(descElement);
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
			wptElement.appendChild(linkElement);
		}
	}
}

void GPXWaypoint::readXml(QDomElement &wptElement)
{
	if (!wptElement.hasAttribute("lat") ||
		!wptElement.hasAttribute("lon"))
	{
		qDebug() << "GPXWaypoint: Error: Node does not contain both lat and lon";
		return;
	}

	latitude_ = wptElement.attribute("lat").toDouble();
	longitude_ = wptElement.attribute("lon").toDouble();

	//qDebug() << "GPXWaypoint: Lat: " << latitude_ << "  Lon: " << longitude_;

	QDomElement child = wptElement.firstChildElement();
	while (!child.isNull())
	{
		if (child.nodeName() == "time")
		{
			QString timeStr = child.childNodes().at(0).nodeValue();
			//qDebug() << "GPXWaypoint: Found time node: " << timeStr;
			setTime(GPXDateTime::fromString(timeStr));
		}

		child = child.nextSiblingElement();
	}

	// TODO: Read other data
}
