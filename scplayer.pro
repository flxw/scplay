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
    soundlistitem.cpp \
    likelistmodel.cpp \
    soundcloudapi.cpp \
    enterusernamewidget.cpp \
    playerwidget.cpp

HEADERS  += mainwindow.h \
    soundmanager.h \
    soundlistmodel.h \
    soundlistitem.h \
    likelistmodel.h \
    soundcloudapi.h \
    enterusernamewidget.h \
    playerwidget.h

FORMS    += mainwindow.ui \
    enterusernamewidget.ui \
    playerwidget.ui

RESOURCES += \
    resources.qrc
