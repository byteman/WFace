#include "pollhandler.h"
#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>

PollerHandler::PollerHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_start(1),
    m_end(0),
    m_start_us(30000),
    m_stop_us(1000000),
    m_quit(false)
{

}

bool PollerHandler::doWork()
{
    qDebug() << "PollerHandler doWork";
    if(_rtu)
    {
        for(quint8 addr = m_start; addr <= m_end;  addr++)
        {
            quint16 values[8];
            _rtu->setDeviceAddr(addr);

            if(4 == _rtu->read_registers(0,4,values))
            {

                emit weightResult(addr,values[0]+(values[1]<<16),values[2],values[3],values[4]+(values[5]<<16),values[6] +( values[7]<<16 ) );
            }
            else{
                //超时.
                qDebug() << "addr" << addr << " timeout";
                emit timeout(addr);
            }
            if(m_quit)
            {
                qDebug() << "PollerHandler quit";
                return true;
            }
            this->msleep(10);

        }

        //this->msleep(100);
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

//最大超时时间100ms,
bool PollerHandler::startRun()
{
    _rtu->set_response_timeout(m_start_us);
    m_quit = false;
    return CmdHandler::startRun();
}

bool PollerHandler::stop()
{
    m_quit = true;
    CmdHandler::stop();
    _rtu->set_response_timeout(m_stop_us);
    return true;
}
