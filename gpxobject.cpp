#include "gpxobject.h"
#include "gpxtypes.h"

GPXObject::GPXObject(QObject *parent) :
	QObject(parent)
{
}

void GPXObject::saveToFile(const QString fileName) const
{
}

GPXObject * GPXObject::loadFromFile(const QString fileName)
{
	return NULL;
}

void GPXObject::exportKML(const QString fileName) const
{
}

GPXObject * GPXObject::importKML(QString fileName)
{
	return NULL;
}

void GPXObject::exportCSV(const QString fileName) const
{
}
