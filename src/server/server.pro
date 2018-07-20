QT -= gui
QT += core multimedia network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    locatingservice.cpp \
    playerservice.cpp \
    windowsservice.cpp \
    fileservice.cpp \
    cuesheet.cpp \
    cuesheetfileentry.cpp \
    cuesheettrackentry.cpp

HEADERS += \
    locatingservice.h \
    playerservice.h \
    windowsservice.h \
    fileservice.h \
    cuesheet.h \
    cuesheetfileentry.h \
    cuesheettrackentry.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ozlib/release/ -lozlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ozlib/debug/ -lozlib
else:unix: LIBS += -L$$OUT_PWD/../ozlib/ -lozlib

INCLUDEPATH += $$PWD/../ozlib
DEPENDPATH += $$PWD/../ozlib


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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/qslog/release/ -lqslog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/qslog/debug/ -lqslog
else:unix: LIBS += -L$$OUT_PWD/../third_party/qslog/ -lqslog

INCLUDEPATH += $$PWD/../third_party/qslog
DEPENDPATH += $$PWD/../third_party/qslog
