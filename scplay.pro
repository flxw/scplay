#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T16:33:14
#
#-------------------------------------------------

QT       += network multimedia widgets core gui

TARGET = scplay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    likelistmodel.cpp \
    soundcloudapi.cpp \
    enterusernamewidget.cpp \
    playerwidget.cpp \
    sounditem.cpp \
    listitem.cpp \
    datastore.cpp \
    mylistmodel.cpp \
    introwidget.cpp \
    soundlistdelegate.cpp \
    playbackmanager.cpp

HEADERS  += mainwindow.h \
    likelistmodel.h \
    soundcloudapi.h \
    enterusernamewidget.h \
    playerwidget.h \
    sounditem.h \
    listitem.h \
    datastore.h \
    mylistmodel.h \
    introwidget.h \
    soundlistdelegate.h \
    playbackmanager.h

FORMS    += mainwindow.ui \
    enterusernamewidget.ui \
    playerwidget.ui \
    introwidget.ui

RESOURCES += \
    resources.qrc

OTHER_FILES +=
