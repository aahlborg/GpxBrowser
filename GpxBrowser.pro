#-------------------------------------------------
#
# Project created by QtCreator 2012-03-16T23:08:32
#
#-------------------------------------------------

QT       += core gui xml

TARGET = GpxBrowser
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
	gpxobject.cpp \
	gpxwaypoint.cpp \
	gpxroutebase.cpp \
	gpxroute.cpp \
	gpxtrack.cpp \
	gpxpath.cpp \
	gpxutilities.cpp \
    gpxmetadata.cpp \
    gpxdatetime.cpp

HEADERS  += mainwindow.h \
	gpxtypes.h \
	gpxwaypoint.h \
	gpxobject.h \
	gpxroutebase.h \
	gpxroute.h \
	gpxtrack.h \
	gpxpath.h \
	gpxutilities.h \
    gpxmetadata.h \
    gpxdatetime.h

FORMS    += mainwindow.ui
