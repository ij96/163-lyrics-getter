QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 163-lyrics-getter
TEMPLATE = app
RC_FILE = icon/icon.rc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    song.cpp

HEADERS += \
    mainwindow.h \
    song.h

TRANSLATIONS += \
    language\zh_CN.ts

RESOURCES += \
    language.qrc
