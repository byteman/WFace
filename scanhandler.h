#ifndef SCANHANDLER_H
#define SCANHANDLER_H

#include "cmdhandler.h"
class ScanHandler : public CmdHandler
{
     Q_OBJECT
public:
    ScanHandler(RtuReader*  rtu);
    bool init(int reg_addr,int reg_size,int max_addr,bool findOne=true);

signals:
    void scanResult(int type, int addr);
private:
    int m_addr;
    int m_reg_addr;
    int m_reg_size;
    int m_max_addr;
    bool m_findone;
    bool m_stop;
    // CmdHandler interface
public:
    virtual bool doWork();
    bool stop();
};

#endif // SCANHANDLER_H
