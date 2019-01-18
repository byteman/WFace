#ifndef SCANHANDLER_H
#define SCANHANDLER_H

#include "cmdhandler.h"
#include <QVector>
enum SCAN_EVENT{
    SCAN_PROGRASS=0, //超时
    SCAN_FIND, //找到一个
    SCAN_COMPLETE //全部地址搜索完成.
};
class ScanHandler : public CmdHandler
{
     Q_OBJECT
public:
    ScanHandler(ModbusReader*  rtu);
    QVector<int>  getAddrList();
    bool init(int reg_addr,int reg_size,int min_addr=1,int max_addr=33,bool findOne=true);

signals:
    void scanResult(int type, int addr);
    void weightResult(int addr,int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
    void timeout(int addr);

private:
    int m_cur_index;
    int m_cur_addr;
    int m_addr;
    int m_reg_addr;
    int m_reg_size;
    int m_start_addr;
    int m_end_addr;
    int m_start_us,m_stop_us;
    bool m_find_done;
    bool m_quit;
    bool m_findOnce; //只搜索最先找到的第一个.
    QVector<int> m_addrArr; //搜索出来的设备地址.
    QVector<int> m_clear_addr; //清零地址.
    QMutex mutex;
    // CmdHandler interface
    bool poll();
    bool scan();
    bool SetQuit();
    void clear();
public:
    virtual bool doWork();
    bool stop();
    void setTimeOut(int startUs, int stopUs);
    bool setZero(int addr);
    bool stopScan();
};

#endif // SCANHANDLER_H
