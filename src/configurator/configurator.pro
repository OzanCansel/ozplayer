#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T17:23:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = configurator
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 

win32:RC_ICONS += app.ico

RESOURCES += \
    res.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../third_party/qtservice/lib/ -lQtSolutions_Service-head
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../third_party/qtservice/lib/ -lQtSolutions_Service-headd

INCLUDEPATH += $$PWD/../third_party/qtservice/include
DEPENDPATH += $$PWD/../third_party/qtservice/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../third_party/qtservice/lib/libQtSolutions_Service-head.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../third_party/qtservice/lib/libQtSolutions_Service-headd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../third_party/qtservice/lib/QtSolutions_Service-head.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../third_party/qtservice/lib/QtSolutions_Service-headd.lib

unix:!macx: LIBS += -L$$PWD/../third_party/qtservice/lib/ -lQtSolutions_Service-head

INCLUDEPATH += $$PWD/../third_party/qtservice/include
DEPENDPATH += $$PWD/../third_party/qtservice/include
