#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include <QThread>
#include "ModbusChannel.h"
#include "ModbusReader.h"

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
    CmdHandler(ModbusReader* rtu,QObject* parent=NULL):
        QThread(parent),
        _rtu(rtu),
        m_reader_index(0),
        bInit(false)
    {

    }
//    CmdHandler(QList<ModbusReader*> rtuList,QObject* parent=NULL):
//        QThread(parent),
//        _rtuList(rtuList),
//        _rtu(NULL),
//        m_reader_index(0),
//        bInit(false)
//    {
//        if(rtuList.count() > 0){
//            _rtu = rtuList.at(0);
//        }
//    }
    void processCmds();
    bool postCmd(RegCmd cmd);
    bool postWriteRegs(int reg_addr, int reg_num, quint16* values);
    bool postReadRegs(int reg_addr, int reg_num);
    bool reInit();

    void ChangeCurrentReader(ModbusReader* reader);
    virtual bool startRun();
    virtual bool stop();
    virtual void run();
    virtual bool doWork();
    virtual bool init();

    //void SetReaderList(QList<RtuReader *> rtuList);
    bool WriteCtrlCmd(int reg, quint8 value);
signals:
    void OperationResult(RegCmd value);
protected:
    bool bInit;
    int   m_reader_index;
    ModbusReader* _rtu;
    //QList<RtuReader*> _rtuList;
    QList<RegCmd> cmdlist;
};

#endif // CMDHANDLER_H
