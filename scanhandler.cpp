#include "scanhandler.h"

#include <qdebug.h>

ScanHandler::ScanHandler(RTU_Modbus *rtu):
    CmdHandler(rtu),
    m_addr(32)
{

}


bool ScanHandler::run()
{
    if(m_addr < 34)
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
            }
        }
        m_addr++;
        return true;
    }
    m_addr = 32;
     emit scanResult(1,m_addr);
    return false;
}

bool ScanHandler::stop()
{
    m_addr = 32;
    return true;
}
