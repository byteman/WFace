#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QObject>
#include "mymodbus.h"

struct RegCmd{
    RegCmd()
    {
        isRead = false;
    }
    bool    isRead;
    int     reg_addr;
    int     reg_num;
    quint16 reg_value[32];
};

class CmdHandler:public QObject
{
    Q_OBJECT
public:
    CmdHandler(RTU_Modbus* rtu,QObject* parent=NULL): QObject(parent),_rtu(rtu)
    {

    }
    void writeCmds();
    bool addCmd(RegCmd cmd);
    virtual bool stop();
    virtual bool run();
protected:
    RTU_Modbus* _rtu;
    QList<RegCmd> cmdlist;
};

#endif // CMDHANDLER_H
