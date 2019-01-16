#-------------------------------------------------
#
# Project created by QtCreator 2018-12-10T11:30:34
#
#-------------------------------------------------

QT       += core gui

QT += sql

QT += printsupport

TRANSLATIONS = cn.ts
TRANSLATIONS += en.ts

#Disable debug when relase
Release:DEFINES += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GenieConverter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    dstringlistmodel.cpp \
    dprinttopdf.cpp \
    daboutdialog.cpp \
    dsetdialog.cpp \
    dbuildtable.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    dstringlistmodel.h \
    dprinttopdf.h \
    daboutdialog.h \
    dsetdialog.h \
    dbuildtable.h \
    config.h

FORMS    += mainwindow.ui \
    dsetdialog.ui

RESOURCES += \
    res.qrc \
    language.qrc


RC_FILE += re.rc
