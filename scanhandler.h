#ifndef SCANHANDLER_H
#define SCANHANDLER_H

#include "cmdhandler.h"
class ScanHandler : public CmdHandler
{
     Q_OBJECT
public:
    ScanHandler(RTU_Modbus*  rtu);
signals:
    void scanResult(int type, int addr);
private:
    int m_addr;

    // CmdHandler interface
public:
    bool run();
    bool stop();
};

#endif // SCANHANDLER_H
