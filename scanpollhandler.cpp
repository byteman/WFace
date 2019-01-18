#include "scanpollhandler.h"
#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>

ScanPollerHandler::ScanPollerHandler(ModbusReader *rtu):
    CmdHandler(rtu),
    m_start(0),
    m_end(0),
    m_cur_index(0),
    m_start_us(30000),
    m_stop_us(1000000),
    m_read_delay_ms(10),
    m_quit(false)
{

}
void ScanPollerHandler::calcFps(void)
{
    static int total = 0;
    static qint64 start = 0;
    if(total == 0){

        start = QDateTime::currentMSecsSinceEpoch();

    }
    total++;
    //int fps = (QDateTime::currentMSecsSinceEpoch() - start) / total;
    qDebug() << "total = " <<  total;

}
bool ScanPollerHandler::canRead()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if( (now - m_last_time ) < m_read_delay_ms)
    {

        return false;
    }
    m_last_time = now;
    return true;

}
bool ScanPollerHandler::doWork()
{
    //qDebug() << "ScanPollerHandler doWork";
    if(_rtu)
    {
        if( m_cur_index < m_addrArr.size())
        {
            quint16 values[8];
            m_cur_addr = m_addrArr[m_cur_index];
            if(!canRead())
            {
                msleep(1);
                return false;
            }
            _rtu->setDeviceAddr(m_cur_addr);

            if(4 == _rtu->read_registers(0,4,values))
            {
                emit weightResult(m_cur_addr,values[0]+(values[1]<<16),values[2],values[3],values[4]+(values[5]<<16),values[6] +( values[7]<<16 ) );
                this->msleep(5);
            }
            else{
                //超时.
                qDebug() << "addr" << m_cur_addr << " timeout";
                emit timeout(m_cur_addr);
            }
            m_cur_index++;

        }else{
            m_cur_index = 0;
        }

        if(m_quit)
        {
            m_cur_index = 0;
            qDebug() << "ScanPollerHandler quit";
            return true;
        }
        return false;
    }
    return true;
}

bool ScanPollerHandler::setAddrSpan(qint8 startAddr, qint8 num)
{

    m_start = startAddr;
    m_end   = startAddr + num;

    return true;
}

void ScanPollerHandler::getAddrSpan(qint8 &startAddr, qint8 &num)
{
    startAddr = m_start;
    num = (m_end - startAddr);
}

void ScanPollerHandler::setTimeOut(int startUs, int stopUs)
{
    m_start_us = startUs;
    m_stop_us = stopUs;
    if(_rtu){
        _rtu->set_response_timeout(startUs);
    }
}
void ScanPollerHandler::setReadInterval(int ms)
{
    m_read_delay_ms = ms;
}

void ScanPollerHandler::setAddrSpan(QVector<int> addrArr)
{
    m_addrArr = addrArr;
}

//最大超时时间100ms,
bool ScanPollerHandler::startRun()
{
    if(_rtu){
        _rtu->set_response_timeout(m_start_us);
    }
    m_quit = false;
    m_last_time =QDateTime::currentMSecsSinceEpoch();
    return CmdHandler::startRun();
}

bool ScanPollerHandler::stop()
{
    m_quit = true;
    CmdHandler::stop();
    if(_rtu){
    _rtu->set_response_timeout(m_stop_us);
    }
    return true;
}
