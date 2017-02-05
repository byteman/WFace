#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QThread>
#include "mymodbus.h"
#include "RtuReader.h"

typedef enum {
    REG_ERROR_OK=0,
    REG_ERROR_TIMEOUT
}RegError;
struct RegCmd{
    RegCmd()
    {
        isRead = false;
        error  = REG_ERROR_OK;
    }
    bool    isRead;
    int     reg_addr;
    int     reg_num;
    int     error; //错误码 0成功 1超时.
    quint16 reg_value[32];

};
class CmdHandler:public QThread
{
    Q_OBJECT
public:
    CmdHandler(RtuReader* rtu,QObject* parent=NULL):
        QThread(parent),
        _rtu(rtu),
        bInit(false)
    {

    }
    void processCmds();
    bool postCmd(RegCmd cmd);
    bool postWriteRegs(int reg_addr, int reg_num, quint16* values);
    bool postReadRegs(int reg_addr, int reg_num);

    virtual bool startRun();
    virtual bool stop();
    virtual void run();
    virtual bool doWork();
    virtual bool init();
signals:
    void OperationResult(RegCmd value);
protected:
    bool bInit;
    RtuReader* _rtu;
    QList<RegCmd> cmdlist;
};

#endif // CMDHANDLER_H
