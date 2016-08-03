#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QObject>
#include "mymodbus.h"
class CmdHandler:public QObject
{
    Q_OBJECT
public:
    CmdHandler(RTU_Modbus* rtu,QObject* parent=NULL): QObject(parent),_rtu(rtu)
    {

    }
    virtual bool stop();
    virtual bool run();
protected:
    RTU_Modbus* _rtu;
};

#endif // CMDHANDLER_H
