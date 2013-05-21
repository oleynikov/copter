#-------------------------------------------------
#
# Project created by QtCreator 2013-05-19T17:37:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CopterController
TEMPLATE = app


SOURCES += main.cpp \
    rs232.c

HEADERS  += \
    CopterController.h \
    rs232.h

FORMS    += \
    CopterController.ui
