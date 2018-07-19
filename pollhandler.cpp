#include "pollhandler.h"
#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>

PollerHandler::PollerHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_start(0),
    m_end(0),
    m_cur_addr(0),
    m_start_us(30000),
    m_stop_us(1000000),
    m_read_delay_ms(10),
    m_quit(false)
{

}
void PollerHandler::calcFps(void)
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
bool PollerHandler::canRead()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if( (now - m_last_time ) < m_read_delay_ms)
    {

        return false;
    }
    m_last_time = now;
    return true;

}
bool PollerHandler::doWork()
{
    //qDebug() << "PollerHandler doWork";
    if(_rtu)
    {


        if( m_cur_addr < m_end)
        {
            quint16 values[8];
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
            m_cur_addr++;

        }
        if(m_cur_addr >= m_end)
        {
            m_cur_addr = m_start;
            calcFps();
        }
        if(m_quit)
        {
            qDebug() << "PollerHandler quit";
            return true;
        }
        return false;
    }
    return true;
}
bool PollerHandler::WriteCtrlCmd(int reg, quint8 value)
{

    RegCmd cmd;
    cmd.reg_addr = reg;
    cmd.reg_num = 1;
    cmd.reg_value[0] = value;
    return postCmd(cmd);
}


int PollerHandler::getDot()
{
    return m_dot;
}

bool PollerHandler::setAddrSpan(qint8 startAddr, qint8 num)
{

    m_start = startAddr;
    m_end   = startAddr + num;

    return true;
}

void PollerHandler::getAddrSpan(qint8 &startAddr, qint8 &num)
{
    startAddr = m_start;
    num = (m_end - startAddr);
}

void PollerHandler::setTimeOut(int startUs, int stopUs)
{
    m_start_us = startUs;
    m_stop_us = stopUs;
    _rtu->set_response_timeout(startUs);
}

void PollerHandler::setReadInterval(int ms)
{
    m_read_delay_ms = ms;
}

//最大超时时间100ms,
bool PollerHandler::startRun()
{
    _rtu->set_response_timeout(m_start_us);
    m_quit = false;
    m_last_time =QDateTime::currentMSecsSinceEpoch();
    return CmdHandler::startRun();
}

bool PollerHandler::stop()
{
    m_quit = true;
    CmdHandler::stop();
    _rtu->set_response_timeout(m_stop_us);
    return true;
}
