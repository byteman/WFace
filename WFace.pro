#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T21:36:46
#
#-------------------------------------------------

QT       += core gui
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
    mymodbus.cpp \
    cmdhandler.cpp \
    scanhandler.cpp \
    weighthandler.cpp \
    parahandler.cpp \
    calibhandler.cpp \
    RtuReader.cpp \
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
    QComSettingWidget.cpp \
    readerwidget.cpp \
    TcpSettingWidget.cpp \
    UartSettingWidget.cpp \
    WeightWidget.cpp \
    CalibWidget.cpp \
    ParamWidget.cpp \
    CornFixWidget.cpp \
    PollerWidget.cpp \
    WaveAnalysisWidget.cpp \
    BaseWidget.cpp \
    WorkerWidgetManager.cpp

HEADERS  += mainwindow.h \
    3rdparty/libmodbus/src/modbus.h \
    mymodbus.h \
    cmdhandler.h \
    scanhandler.h \
    weighthandler.h \
    parahandler.h \
    calibhandler.h \
    RtuReader.h \
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
    UartSettingWidget.h \
    QComSettingWidget.h \
    readerwidget.h \
    TcpSettingWidget.h \
    WeightWidget.h \
    CalibWidget.h \
    ParamWidget.h \
    CornFixWidget.h \
    PollerWidget.h \
    WaveAnalysisWidget.h \
    BaseWidget.h \
    WorkerWidgetManager.h

INCLUDEPATH += 3rdparty/libmodbus 3rdparty/libmodbus/src

INCLUDEPATH += 3rdparty/poco/include
win32:INCLUDEPATH += 3rdparty/poco/include\ 3rdparty/pcomm

msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

win32:DEFINES += _TTY_WIN_  WINVER=0x0501
win32:LIBS += -L$$PWD/3rdparty/pcomm/ -lPCOMM
win32:LIBS += -lsetupapi -lwsock32 -lws2_32 -lAdvapi32
win32:LIBS += -L./3rdparty/poco/lib
LIBS += -lOpengl32 \
                -lglu32
FORMS    += mainwindow.ui \
    devwidget.ui \
    dialogwave.ui \
    QComSettingWidget.ui \
    TcpSettingWidget.ui \
    UartSettingWidget.ui \
    WeightWidget.ui \
    CalibWidget.ui \
    ParamWidget.ui \
    CornFixWidget.ui \
    PollerWidget.ui \
    WaveAnalysisWidget.ui
RESOURCES += \
    myrc.qrc
TRANSLATIONS+=cn.ts en.ts
RC_FILE=icon.rc
DISTFILES +=
