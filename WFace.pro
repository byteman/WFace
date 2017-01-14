#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T21:36:46
#
#-------------------------------------------------

QT       += core gui
QT += network serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WFace
TEMPLATE = app
#QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

SOURCES += main.cpp\
        mainwindow.cpp \
        3rdparty/libmodbus/src/modbus.c \
        3rdparty/libmodbus/src/modbus-data.c \
        3rdparty/libmodbus/src/modbus-rtu.c \
        3rdparty/libmodbus/src/modbus-tcp.c \
    mymodbus.cpp \
    adc102.cpp \
    cmdhandler.cpp \
    scanhandler.cpp \
    weighthandler.cpp \
    parahandler.cpp \
    calibhandler.cpp \
    updatehandler.cpp \
    paramconfig.cpp \
    netclient.cpp \
    networkmgr.cpp \
    crc16.cpp \
    timerworker.cpp \
    customparams.cpp

HEADERS  += mainwindow.h \
    3rdparty/libmodbus/src/modbus.h \
    mymodbus.h \
    adc102.h \
    cmdhandler.h \
    scanhandler.h \
    weighthandler.h \
    parahandler.h \
    calibhandler.h \
    updatehandler.h \
    paramconfig.h \
    wcommon.h \
    cwSCSqlite.h \
    netclient.h \
    networkmgr.h \
    netmsg.h \
    timerworker.h \
    customparams.h

INCLUDEPATH += 3rdparty/libmodbus 3rdparty/libmodbus/src

INCLUDEPATH += 3rdparty/poco/include
win32:INCLUDEPATH += 3rdparty/poco/include\ 3rdparty/pcomm


win32:DEFINES += _TTY_WIN_  WINVER=0x0501
win32:LIBS += -L$$PWD/3rdparty/pcomm/ -lPCOMM
win32:LIBS += -lsetupapi -lwsock32 -lws2_32 -lAdvapi32
#win32:LIBS += -L./3rdparty/poco/lib/win32

FORMS    += mainwindow.ui

RESOURCES += \
    myrc.qrc
TRANSLATIONS+=cn.ts
RC_FILE=icon.rc
DISTFILES +=
