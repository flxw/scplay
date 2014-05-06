#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T16:33:14
#
#-------------------------------------------------

include(o2/o2.pri)

QT       += network multimedia widgets core gui

TARGET = scplay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    soundcloudapi.cpp \
    enterusernamewidget.cpp \
    playerwidget.cpp \
    introwidget.cpp \
    soundlistdelegate.cpp \
    playbackmanager.cpp \
    sound.cpp \
    playlist.cpp \
    likemodel.cpp \
    soundstorage.cpp \
    playlistmodel.cpp \
    listmodelbase.cpp \
    activitylistmodel.cpp \
    playlistsoundlistmodel.cpp

HEADERS  += mainwindow.h \
    soundcloudapi.h \
    enterusernamewidget.h \
    playerwidget.h \
    introwidget.h \
    soundlistdelegate.h \
    playbackmanager.h \
    sound.h \
    playlist.h \
    likemodel.h \
    soundstorage.h \
    playlistmodel.h \
    listmodelbase.h \
    activitylistmodel.h \
    playlistsoundlistmodel.h

FORMS    += mainwindow.ui \
    enterusernamewidget.ui \
    playerwidget.ui \
    introwidget.ui

RESOURCES += \
    resources.qrc
