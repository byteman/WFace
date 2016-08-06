#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T21:36:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WFace
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    3rdparty/qextserialport/qextserialport.cpp	\
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
    calibhandler.cpp

HEADERS  += mainwindow.h \
    3rdparty/qextserialport/qextserialport.h \
    3rdparty/qextserialport/qextserialenumerator.h \
    3rdparty/libmodbus/src/modbus.h \
    mymodbus.h \
    adc102.h \
    cmdhandler.h \
    scanhandler.h \
    weighthandler.h \
    parahandler.h \
    calibhandler.h

INCLUDEPATH += 3rdparty/libmodbus 3rdparty/libmodbus/src 3rdparty/qextserialport

win32:INCLUDEPATH += 3rdparty/poco/include\
unix:SOURCES += 3rdparty/qextserialport/posix_qextserialport.cpp	\
                3rdparty/qextserialport/qextserialenumerator_unix.cpp
unix:DEFINES += _TTY_POSIX_
win32:SOURCES += 3rdparty/qextserialport/win_qextserialport.cpp \
                        3rdparty/qextserialport/qextserialenumerator_win.cpp
win32:DEFINES += _TTY_WIN_  WINVER=0x0501
win32:LIBS += -lsetupapi -lwsock32 -lws2_32 -lAdvapi32
win32:LIBS += -L./3rdparty/poco/lib

FORMS    += mainwindow.ui

RESOURCES += \
    myrc.qrc
TRANSLATIONS+=cn.ts
RC_FILE=icon.rc
DISTFILES +=
