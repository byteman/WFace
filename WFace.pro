#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T21:36:46
#
#-------------------------------------------------

QT       += core gui network
QT +=  serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = WFace
TEMPLATE = app

DEFINES += QCUSTOMPLOT_USE_OPENGL
SOURCES += main.cpp\
        mainwindow.cpp \
        3rdparty/libmodbus/src/modbus.c \
        3rdparty/libmodbus/src/modbus-data.c \
        3rdparty/libmodbus/src/modbus-rtu.c \
        3rdparty/libmodbus/src/modbus-tcp.c \
    cmdhandler.cpp \
    scanhandler.cpp \
    weighthandler.cpp \
    parahandler.cpp \
    calibhandler.cpp \
    cornhandler.cpp \
    pollhandler.cpp \
    mydevices.cpp \
    devwidget.cpp \
    utils.cpp \
    ifilter.cpp \
    igraphicchannel.cpp \
    ilinechart.cpp \
    qcustomchart.cpp \
    qcustomplot.cpp \
    qcustomplotchannel.cpp \
    wavefile.cpp \
    wavewidget.cpp \
    dialogwave.cpp \
    myconfig.cpp \
    csvfile.cpp \
    ModbusRtu.cpp \
    ModbusTcp.cpp \
    ModbusChannel.cpp \
    ModbusReader.cpp \
    SingleModbusReader.cpp \
    MultiModbusReader.cpp \
    ModbusReaderFactory.cpp \
    MbServer.cpp

HEADERS  += mainwindow.h \
    3rdparty/libmodbus/src/modbus.h \
    mymodbus.h \
    cmdhandler.h \
    scanhandler.h \
    weighthandler.h \
    parahandler.h \
    calibhandler.h \
    cornhandler.h \
    pollhandler.h \
    mydevices.h \
    devwidget.h \
    utils.h \
    ifilter.h \
    igraphicchannel.h \
    ilinechart.h \
    qcustomchart.h \
    qcustomplot.h \
    qcustomplotchannel.h \
    wavefile.h \
    wavewidget.h \
    dialogwave.h \
    myconfig.h \
    csvfile.h \
    ModbusRtu.h \
    ModbusTcp.h \
    ModbusChannel.h \
    ModbusReader.h \
    SingleModbusReader.h \
    MultiModbusReader.h \
    ModbusReaderFactory.h \
    MbServer.h

INCLUDEPATH += 3rdparty/libmodbus 3rdparty/libmodbus/src

INCLUDEPATH += 3rdparty/poco/include
win32:INCLUDEPATH += 3rdparty/poco/include\ 3rdparty/pcomm

INCLUDEPATH += ./ext/qBreakpad/handler
LIBS+=-Lext/lib
LIBS += -Lext/qBreakpad/handler

CONFIG(debug, debug|release) {
        LIBS+=-lqBreakpadd

}

CONFIG(release, debug|release) {
        LIBS+=-lqBreakpad
        QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
win32:DEFINES += _TTY_WIN_  WINVER=0x0501
win32:LIBS += -L$$PWD/3rdparty/pcomm/ -lPCOMM
win32:LIBS += -lsetupapi -lwsock32 -lws2_32 -lAdvapi32
win32:LIBS += -L./3rdparty/poco/lib
LIBS += -lOpengl32 \
                -lglu32
FORMS    += mainwindow.ui \
    devwidget.ui \
    dialogwave.ui

RESOURCES += \
    myrc.qrc
TRANSLATIONS+=cn.ts en.ts
RC_FILE=icon.rc
DISTFILES +=
