#ifndef SCANHANDLER_H
#define SCANHANDLER_H

#include "cmdhandler.h"
enum SCAN_EVENT{
    SCAN_PROGRASS=0, //超时
    SCAN_FIND, //找到一个
    SCAN_COMPLETE //全部地址搜索完成.
};
class ScanHandler : public CmdHandler
{
     Q_OBJECT
public:
    ScanHandler(RtuReader*  rtu);
    bool init(int reg_addr,
              int reg_size,
              int slave_id=1,
              int min_addr=1,
              int max_addr=33,
              bool findOne=true);

signals:
    void scanResult(int type, int addr);
private:
    int m_addr;
    int m_reg_addr;
    int m_reg_size;
    int m_slave_id;
    int m_start_addr;
    int m_end_addr;
    int m_start_us,m_stop_us;
    bool m_findOnce; //只搜索最先找到的第一个.
    // CmdHandler interface
    bool read_sensor_num();
public:
    virtual bool doWork();
    bool stop();
    void setTimeOut(int startUs, int stopUs);
};

#endif // SCANHANDLER_H
