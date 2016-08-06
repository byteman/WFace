#include "scanhandler.h"

#include <qdebug.h>

ScanHandler::ScanHandler(RTU_Modbus *rtu):
    CmdHandler(rtu),
    m_addr(1),
    m_findone(true)
{

}

bool ScanHandler::startScan(bool findOne)
{
    m_findone = findOne;
    return true;
}


bool ScanHandler::run()
{
    if(m_addr < 33)
    {
        if(_rtu)
        {
            quint16 state;

            _rtu->setDeviceAddr(m_addr);
            int len = _rtu->read_registers(3,1,&state);
            qDebug() << "addr=" << m_addr << " len=" << len;
            if(len == 1)
            {
                emit scanResult(0,m_addr);
                if(m_findone)
                {
                    m_addr = 1;
                    emit scanResult(1,m_addr);
                    return false;
                }
            }
        }
        m_addr++;
        return true;
    }
    m_addr = 1;
     emit scanResult(1,m_addr);
    return false;
}

bool ScanHandler::stop()
{
    m_addr = 32;
    return true;
}
