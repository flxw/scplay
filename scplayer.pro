#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T16:33:14
#
#-------------------------------------------------

QT       += network multimedia widgets core gui

TARGET = scplayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    soundmanager.cpp

HEADERS  += mainwindow.h \
    soundmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
