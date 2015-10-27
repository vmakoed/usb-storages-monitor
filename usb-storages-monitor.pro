#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T19:35:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usb-storages-monitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drivesscanner/drivesscanner.cpp \
    drivesscanner/getStoragesInfo.cpp \
    drivewidget.cpp \
    drive.cpp \
    drivesejector/RemoveDriveByLetter.cpp \
    drivesejector/drivesejector.cpp

HEADERS  += mainwindow.h \
    drivesscanner/drivesscanner.h \
    drivesscanner/getStoragesInfo.h \
    drivesscanner/storageinfo.h \
    drivewidget.h \
    drive.h \
    drivesejector/RemoveDriveByLetter.h \
    drivesejector/drivesejector.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    res/resources.qrc
