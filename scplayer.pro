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
    soundmanager.cpp \
    soundlistmodel.cpp \
    soundlistitem.cpp

HEADERS  += mainwindow.h \
    soundmanager.h \
    soundlistmodel.h \
    soundlistitem.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
