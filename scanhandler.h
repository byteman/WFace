#ifndef SCANHANDLER_H
#define SCANHANDLER_H

#include "cmdhandler.h"
class ScanHandler : public CmdHandler
{
     Q_OBJECT
public:
    ScanHandler(RTU_Modbus*  rtu);
    bool startScan(bool findOne=true);
signals:
    void scanResult(int type, int addr);
private:
    int m_addr;
    bool m_findone;
    // CmdHandler interface
public:
    void run();
    bool stop();
};

#endif // SCANHANDLER_H
