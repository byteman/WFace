#include "scanhandler.h"

#include <qdebug.h>

ScanHandler::ScanHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_addr(1),
    m_findone(true),
    m_stop(true)
{

}

bool ScanHandler::init(int reg_addr,int reg_size,int max_addr,bool findOne)
{
    m_addr = 1;
    m_findone = findOne;
    m_stop = false;
    m_reg_addr = reg_addr;
    m_reg_size = reg_size;
    m_max_addr = max_addr;
    return true;
}


bool ScanHandler::doWork()
{
    if(m_addr < m_max_addr )
    {
        if(_rtu)
        {
            quint16 state;

            _rtu->setDeviceAddr(m_addr);
            int len = _rtu->read_registers(m_reg_addr,m_reg_size,&state);
            qDebug() << "addr=" << m_addr << " len=" << len;
            if(len == 1)
            {
                emit scanResult(0,m_addr);
                if(m_findone)
                {
                    m_addr = 1;
                    emit scanResult(1,m_addr);
                    return true;
                }
            }
        }
        emit scanResult(2,m_addr);
        m_addr++;

        return false;
    }
    m_addr = 1;
    emit scanResult(1,m_addr);
    return true;
}

bool ScanHandler::stop()
{
    m_addr = m_max_addr;
    emit scanResult(1,m_addr);
    this->wait();
    return true;
}
