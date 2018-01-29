#-------------------------------------------------
#
# Project created by QtCreator 2017-12-15T02:51:26
#
#-------------------------------------------------

QT       += core gui multimedia winextras network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = music_player
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        musicplayer.cpp \
    volumebutton.cpp \
    titlebar.cpp \
    qqmusic_api.cpp \
    musiccover.cpp \
    searchwidget.cpp \
    musiccontrol.cpp

HEADERS += \
        musicplayer.h \
    volumebutton.h \
    titlebar.h \
    qqmusic_api.h \
    musiccover.h \
    searchwidget.h \
    musicdata.h \
    musiccontrol.h \
    blur_image.h

FORMS += \
        musicplayer.ui \
    titlebar.ui \
    search.ui \
    musiccover.ui \
    musiccontrol.ui

RESOURCES += \
    rec.qrc
