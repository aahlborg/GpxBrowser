#-------------------------------------------------
#
# Project created by QtCreator 2012-07-19T12:05:13
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = GpxLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += gpxobject.cpp \
    gpxwaypoint.cpp \
    gpxutilities.cpp \
    gpxtrack.cpp \
    gpxroutebase.cpp \
    gpxroute.cpp \
    gpxpath.cpp \
    gpxmetadata.cpp \
    gpxdatetime.cpp

HEADERS += gpxobject.h \
    gpxwaypoint.h \
    gpxutilities.h \
    gpxtypes.h \
    gpxtrack.h \
    gpxroutebase.h \
    gpxroute.h \
    gpxpath.h \
    gpxmetadata.h \
    gpxdatetime.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
