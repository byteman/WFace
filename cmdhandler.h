#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QThread>
#include "mymodbus.h"
#include "RtuReader.h"

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

class CmdHandler:public QThread
{
    Q_OBJECT
public:
    CmdHandler(RtuReader* rtu,QObject* parent=NULL): QThread(parent),_rtu(rtu)
    {

    }
    void writeCmds();
    bool addCmd(RegCmd cmd);
    virtual bool startRun();
    virtual bool stop();
    virtual void run();
    virtual bool doWork();
protected:
    RtuReader* _rtu;
    QList<RegCmd> cmdlist;
};

#endif // CMDHANDLER_H
