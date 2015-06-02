#-------------------------------------------------
#
# Project created by QtCreator 2015-05-30T17:09:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManualTexPack
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    tilesetview.cpp \
    tileset.cpp \
    sprite.cpp \
    spriteloader.cpp

HEADERS  += mainwindow.h \
    tilesetview.h \
    tileset.h \
    sprite.h \
    spriteloader.h

FORMS    += mainwindow.ui
