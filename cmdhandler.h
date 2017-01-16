#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QObject>
#include "mymodbus.h"

struct RegCmd{
    RegCmd()
    {
        retry_num = 3;
    }
    int     retry_num;
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
    bool writeCmds();
    bool addCmd(RegCmd cmd);
    bool addCmd(int reg_addr, int reg_num, quint16 *reg_values);
    virtual bool stop();
    virtual bool myrun();
protected:
    RTU_Modbus* _rtu;
    QList<RegCmd> cmdlist;
};

#endif // CMDHANDLER_H
