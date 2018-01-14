QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 163-lyrics-getter
TEMPLATE = app
RC_FILE = icon/icon.rc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    song.cpp \
    image.cpp \
    togglebutton.cpp \
    lyrics.cpp

HEADERS += \
    mainwindow.h \
    song.h \
    image.h \
    togglebutton.h \
    lyrics.h

TRANSLATIONS += \
    language\zh_CN.ts

RESOURCES += \
    language.qrc
