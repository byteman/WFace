#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QObject>
#include <QThread>
#include "mymodbus.h"
class CmdHandler:public QThread
{
    Q_OBJECT
public:
    CmdHandler(RTU_Modbus* rtu,QObject* parent=NULL): QThread(parent),_rtu(rtu)
    {

    }
    virtual bool start();
    virtual bool stop();
    virtual void run();
protected:
    RTU_Modbus* _rtu;
};

#endif // CMDHANDLER_H
