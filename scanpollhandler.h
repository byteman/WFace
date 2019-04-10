#ifndef SCAN_POLL_HANDLER_H
#define SCAN_POLL_HANDLER_H

#include "cmdhandler.h"

#include <QList>
#include <QTime>
#include <QVector>

class ScanPollerHandler : public CmdHandler
{
     Q_OBJECT
public:
    ScanPollerHandler(ModbusReader*  rtu);

    //ScanPollerHandler(QList<RtuReader*> rtuList);

    virtual bool doWork();

    int  getDot();
    bool  setAddrSpan(qint8 startAddr, qint8 num);
    void  getAddrSpan(qint8 &startAddr, qint8& num);
    void  setTimeOut(int startUs, int stopUs);
    void  setReadInterval(int ms);
    void  setAddrSpan(QVector<int> addrArr);
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
    int m_cur_index;
    int m_cur_addr;
    int m_total_num;
    qint64 m_last_time;
    QVector<int> m_addrArr;
    QMutex m_mutex;
    // CmdHandler interface
    bool canRead();
    void calcFps();
public:
    bool startRun();
    bool stop();
};

#endif // WEIGHTHANDLER_H
