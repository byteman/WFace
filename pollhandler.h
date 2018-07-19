#ifndef POLL_HANDLER_H
#define POLL_HANDLER_H

#include "cmdhandler.h"

#include <QList>
#include <QTime>


class PollerHandler : public CmdHandler
{
     Q_OBJECT
public:
    PollerHandler(RtuReader*  rtu);


    virtual bool doWork();

    int  getDot();
    bool  setAddrSpan(qint8 startAddr, qint8 num);
    void  getAddrSpan(qint8 &startAddr, qint8& num);
    void  setTimeOut(int startUs, int stopUs);
    void  setReadInterval(int ms);
signals:

    void weightResult(int addr,int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
    void timeout(int addr);
private:
    bool paraRead();
    bool WriteCtrlCmd(int reg, quint8 value);
    int  m_dot;
    qint8  m_start,m_end;
    bool m_quit;
    int m_start_us,m_stop_us;
    int m_read_delay_ms;
    int m_cur_addr;
    qint64 m_last_time;
    // CmdHandler interface
    bool canRead();
    void calcFps();
public:
    bool startRun();
    bool stop();
};

#endif // WEIGHTHANDLER_H
