#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T22:28:28
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chess
TEMPLATE = app


SOURCES += main.cpp \
    gameinterface.cpp \
    environment.cpp \
    gameinfo.cpp \
    myobject.cpp \
    music.cpp \
    record.cpp \
    welcomeinterface.cpp \
    switcher.cpp \
    serveraddbox.cpp \
    serverlistbox.cpp \
    tcp.cpp \
    waitingdialog.cpp \
    waitingdialog2.cpp

HEADERS  += \
    gameinterface.h \
    enviroment.h \
    gameinfo.h \
    myobject.h \
    music.h \
    record.h \
    welcomeinterface.h \
    switcher.h \
    serveraddbox.h \
    serverlistbox.h \
    tcp.h \
    waitingdialog.h \
    waitingdialog2.h

FORMS    += \
    gameinterface.ui \
    welcomeinterface.ui \
    serveraddbox.ui \
    serverlistbox.ui \
    waitingdialog.ui \
    waitingdialog2.ui

CONFIG += c++11

RESOURCES += \
    resource.qrc
