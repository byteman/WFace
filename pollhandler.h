#ifndef POLL_HANDLER_H
#define POLL_HANDLER_H

#include "cmdhandler.h"

#include <QList>



class PollerHandler : public CmdHandler
{
     Q_OBJECT
public:
    PollerHandler(RtuReader*  rtu);


    virtual bool doWork();

    int  getDot();
    bool  setAddrSpan(qint8 startAddr, qint8 num);
    void  getAddrSpan(qint8 &startAddr, qint8& num);
signals:

    void weightResult(int addr,int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
private:
    bool paraRead();
    bool WriteCtrlCmd(int reg, quint8 value);
    int  m_dot;
    qint8  m_start,m_end;
};

#endif // WEIGHTHANDLER_H
