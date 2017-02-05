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


void ScanHandler::run()
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
                    return;
                }
            }
        }
        emit scanResult(2,m_addr);
        m_addr++;

        return;
    }
    m_addr = 1;
     emit scanResult(1,m_addr);

}

bool ScanHandler::stop()
{
    m_addr = 33;
    emit scanResult(1,m_addr);
    return true;
}
