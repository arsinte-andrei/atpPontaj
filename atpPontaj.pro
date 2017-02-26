#-------------------------------------------------
#
# Project created by QtCreator 2017-01-13T18:36:23
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = atpPontaj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    atpini.cpp \
    atpdb.cpp \
    dialogoptions.cpp \
    dialogimportattandance.cpp \
    mdijob.cpp \
    mdiworker.cpp \
    mdijobdetails.cpp \
    mdiworkerdetails.cpp \
    mdiattendance.cpp \
    mdipayments.cpp \
    mdisubcontracts.cpp \
    mdiuserrates.cpp \
    mdiusernotes.cpp \
    mdijobdetaileddetails.cpp

HEADERS  += mainwindow.h \
    atpini.h \
    atpdb.h \
    dialogoptions.h \
    dialogimportattandance.h \
    mdijob.h \
    mdiworker.h \
    mdijobdetails.h \
    mdiworkerdetails.h \
    mdiattendance.h \
    mdipayments.h \
    mdisubcontracts.h \
    mdiuserrates.h \
    mdiusernotes.h \
    mdijobdetaileddetails.h

FORMS    += mainwindow.ui \
    dialogoptions.ui \
    dialogimportattandance.ui \
    mdijob.ui \
    mdiworker.ui \
    mdijobdetails.ui \
    mdiworkerdetails.ui \
    mdiattendance.ui \
    mdipayments.ui \
    mdisubcontracts.ui \
    mdiuserrates.ui \
    mdiusernotes.ui \
    mdijobdetaileddetails.ui
