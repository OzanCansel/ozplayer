#-------------------------------------------------
#
# Project created by QtCreator 2018-06-22T23:52:02
#
#-------------------------------------------------

QT       -= gui

TARGET = ozlib
TEMPLATE = lib

CONFIG += c++11

DEFINES += OZLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        lib.cpp \
    listcommand.cpp \
    retrievelistcmd.cpp \
    playcommand.cpp \
    findplayeripcmd.cpp \
    playeripresult.cpp \
    resumecommand.cpp \
    pausecommand.cpp \
    namedcommand.cpp \
    entrylistresult.cpp \
    retrieveentriescommand.cpp \
    entryinfo.cpp \
    currenttracknotify.cpp \
    volumenotify.cpp \
    osinfo.cpp

HEADERS += \
        lib.h \
        lib_global.h \ 
    iserializable.h \
    listcommand.h \
    retrievelistcmd.h \
    playcommand.h \
    findplayeripcmd.h \
    playeripresult.h \
    resumecommand.h \
    pausecommand.h \
    namedcommand.h \
    entrylistresult.h \
    retrieveentriescommand.h \
    entryinfo.h \
    currenttracknotify.h \
    volumenotify.h \
    osinfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
