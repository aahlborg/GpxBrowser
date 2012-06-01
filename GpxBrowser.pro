#-------------------------------------------------
#
# Project created by QtCreator 2012-03-16T23:08:32
#
#-------------------------------------------------

QT       += core gui xml network

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
    gpxdatetime.cpp \
    tileprovider.cpp \
    mapview.cpp \
    tilemanager.cpp \
    downloadmanager.cpp

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
    gpxdatetime.h \
    tileprovider.h \
    mapview.h \
    tilemanager.h \
    downloadmanager.h

FORMS    += mainwindow.ui
