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
    tileprovider.cpp \
    mapview.cpp \
    tilemanager.cpp \
    downloadmanager.cpp

HEADERS  += mainwindow.h \
    tileprovider.h \
    mapview.h \
    tilemanager.h \
    downloadmanager.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GpxLib/release/ -lGpxLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GpxLib/debug/ -lGpxLib
else:unix:!symbian: LIBS += -L$$OUT_PWD/../GpxLib/ -lGpxLib

INCLUDEPATH += $$PWD/../GpxLib
DEPENDPATH += $$PWD/../GpxLib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GpxLib/release/GpxLib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GpxLib/debug/GpxLib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../GpxLib/libGpxLib.a
