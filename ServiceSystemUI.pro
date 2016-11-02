#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T18:18:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServiceSystemUI
TEMPLATE = app

win32:RC_FILE += ServiceSystemUI.rc
win32:RC_ICONS += icon.ico

SOURCES += main.cpp\
        ServiceSystemUI.cpp \
    Buffer.cpp \
    Receiver.cpp \
    Request.cpp \
    ServiceSystem.cpp \
    Source.cpp \
    Coordinates.cpp

HEADERS  += \
    ServiceSystemUI.hpp \
    Buffer.hpp \
    Configuration.hpp \
    Receiver.hpp \
    Request.hpp \
    ServiceSystem.hpp \
    Source.hpp \
    Coordinates.hpp

FORMS    += ServiceSystemUI.ui

DISTFILES += \
    ServiceSystemUI.rc
