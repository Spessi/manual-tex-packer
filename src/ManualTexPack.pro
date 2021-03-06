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
        ui/mainwindow.cpp \
    tilesetview.cpp \
    tileset.cpp \
    sprite.cpp \
    spriteloader.cpp \
    ui/dialognewtileset.cpp \
    project.cpp

HEADERS  += ui/mainwindow.h \
    tilesetview.h \
    tileset.h \
    sprite.h \
    spriteloader.h \
    ui/dialognewtileset.h \
    project.h

FORMS    += ui/mainwindow.ui \
    ui/dialognewtileset.ui
