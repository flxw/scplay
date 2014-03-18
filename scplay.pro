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
    soundmanager.cpp \
    likelistmodel.cpp \
    soundcloudapi.cpp \
    enterusernamewidget.cpp \
    playerwidget.cpp \
    playlist.cpp \
    sounditem.cpp \
    listitem.cpp \
    datastore.cpp \
    mylistmodel.cpp \
    introwidget.cpp

HEADERS  += mainwindow.h \
    soundmanager.h \
    likelistmodel.h \
    soundcloudapi.h \
    enterusernamewidget.h \
    playerwidget.h \
    playlist.h \
    sounditem.h \
    listitem.h \
    datastore.h \
    mylistmodel.h \
    introwidget.h

FORMS    += mainwindow.ui \
    enterusernamewidget.ui \
    playerwidget.ui \
    introwidget.ui

RESOURCES += \
    resources.qrc

OTHER_FILES +=
