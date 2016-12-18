#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T15:26:24
#
#-------------------------------------------------

QT      -= core gui

TARGET = DatabaseModule
TEMPLATE = lib


include( $${PWD}/../qt/common.pri )


win32::DEFINES += DATABASEMODULE_EXPORTS DBCONFIG_EXPORTS

LIBS +=  -lCore -lLogModule
CONFIG(debug, debug|release):LIBS += -lPocoNetd -lPocoUtild -lPocoJSONd -lPocoDataSQLited -lPocoDatad -lPocoFoundationd
else:CONFIG(release, debug|release):LIBS += -lPocoNet -lPocoUtil -lPocoJSON -lPocoDataSQLite -lPocoData -lPocoFoundation


SOURCES += \ 
    cwLog.cpp \
    cwSCSqlite.cpp \
    DatabaseModuleImpl.cpp \
    DBConfiguration.cpp \
    MyJSONConfig.cpp \
    PathUtil.cpp \
    SysPara.cpp

HEADERS += \
    cwLog.h \
    cwSCSqlite.h \
    DatabaseModuleConfig.h \
    DatabaseModuleImpl.h \
    DatabaseModuleInc.h \
    DBConfiguration.h \
    MyJSONConfig.h \
    PathUtil.h \
    SysPara.h
