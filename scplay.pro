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
    soundcloudapi.cpp \
    enterusernamewidget.cpp \
    playerwidget.cpp \
    introwidget.cpp \
    soundlistdelegate.cpp \
    playbackmanager.cpp \
    soundmodel.cpp \
    sound.cpp

HEADERS  += mainwindow.h \
    soundcloudapi.h \
    enterusernamewidget.h \
    playerwidget.h \
    introwidget.h \
    soundlistdelegate.h \
    playbackmanager.h \
    soundmodel.h \
    sound.h

FORMS    += mainwindow.ui \
    enterusernamewidget.ui \
    playerwidget.ui \
    introwidget.ui

RESOURCES += \
    resources.qrc

OTHER_FILES +=
