#-------------------------------------------------
#
# Project created by QtCreator 2019-04-21T16:48:40
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "BoxeeSimulator"
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

CONFIG += c++11 c++14 #c++14 is required by qhttp - it also requires c++11 on MacOS.

SOURCES += \
        core/boxee_net_server.cpp \
        gui/log_view.cpp \
        gui/preferences_dialog.cpp \
        main.cpp \
        gui/main_window.cpp \
        core/boxee.cpp \
        model/net_message.cpp

HEADERS += \
        gui/log_view.h \
        gui/main_window.h \
        core/boxee.h \
        core/boxee_net_server.h \
        gui/preferences_dialog.h \
        model/net_message.h

FORMS += \
    gui/main_window.ui \
    gui/preferences_dialog.ui

QHTTP_DIR = ../3rdparty/qhttp

# Path for qhttp header files (and its dependency, http_parser).
INCLUDEPATH += $$QHTTP_DIR/src $$QHTTP_DIR/3rdparty

# Path for the qhttp static library used by the application.
LIBS += -L$$QHTTP_DIR/xbin -lqhttp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

QMAKE_INFO_PLIST = macosx/Info.plist
