#include "scanhandler.h"

#include <qdebug.h>

ScanHandler::ScanHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_addr(1),
    m_findOnce(true)
{

}

bool ScanHandler::init(int reg_addr,int reg_size,int min_addr,int max_addr,bool findOne)
{
    m_addr = 1;
    m_findOnce = findOne;
    m_reg_addr = reg_addr;
    m_reg_size = reg_size;
    m_end_addr = max_addr;
    m_start_addr = min_addr;
    _rtu->set_response_timeout(100000);
    return true;
}


bool ScanHandler::doWork()
{

    if(m_addr < m_end_addr )
    {
        emit scanResult(SCAN_PROGRASS,m_addr);
        if(_rtu)
        {
            quint16 state;

            _rtu->setDeviceAddr(m_addr);
            int len = _rtu->read_registers(m_reg_addr,m_reg_size,&state);
            qDebug() << "addr=" << m_addr << " len=" << len;
            if(len == m_reg_size)
            {
                emit scanResult(SCAN_FIND,m_addr);
                if(m_findOnce)
                {
                    m_addr = m_start_addr;
                    emit scanResult(SCAN_COMPLETE,m_addr);
                    return true;
                }
            }
        }

        m_addr++;
        //还没有扫描完毕
        return false;
    }
    m_addr = m_start_addr;
    //扫描完毕.
    emit scanResult(SCAN_COMPLETE,m_addr);
    return true;
}

bool ScanHandler::stop()
{
    m_addr = m_end_addr;
    //直接发送扫描完毕.等待线程结束.
    this->wait();
    emit scanResult(SCAN_COMPLETE,m_addr);
    _rtu->set_response_timeout(1000000);
    return true;
}
