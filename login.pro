#-------------------------------------------------
#
# Project created by QtCreator 2020-04-02T23:46:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkit webkitwidgets network

TARGET = login
TEMPLATE = app
CONFIG +=c++11 QT_DEBUG



SOURCES += main.cpp \
    qcustomnetworkreply.cpp \
    websocket_object.cpp

HEADERS  += \
    access_manager.h \
    qcustomnetworkreply.h \
    websocket_object.h

FORMS    +=

LIBS +=

RESOURCES += \
    resource.qrc
