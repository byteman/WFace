#include "RtuReader.h"
#include <QDebug>

RtuReader::RtuReader(QObject *parent) : QObject(parent),
    m_connect(false),
    m_slaveAddr(-1)
{

}

bool RtuReader::setAddr(int addr)
{
    bool ret = true;
    if(addr == -1){
        return false;
    }
    m_slaveAddr = addr;

    m_rtu.setDeviceAddr(addr);
    m_connect = ret;

    return m_connect;
}

bool RtuReader::set_response_timeout(int us)
{
   return m_rtu.set_response_timeout(us);
}

bool RtuReader::start(int interval)
{
    m_interval = interval;
    m_thread.start();
    moveToThread(&m_thread);
    QTimer::singleShot(m_interval,this,SLOT(doWork()));
    return true;

}
bool RtuReader::open(QString port, int baud, char parity, char databit, char stopbit)
{
    if(!m_rtu.open(port.toStdString().c_str(),baud,parity,databit,stopbit))
    {

        return false;
    }
    //m_rtu.set_response_timeout(100000);
    return true;
}
void RtuReader::doWork()
{

    RegOperCmd cmd;
    while(m_cmdlist.size() > 0)
    {
        cmd = m_cmdlist.first();
        if(cmd.isRead)
        {
            if(cmd.reg_num != m_rtu.read_input_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
            {
                cmd.error = REG_ERROR_TIMEOUT;
                emit OperationResult(cmd);
            }
        }
        else
        {
            if(cmd.reg_num != m_rtu.write_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
            {
                cmd.error = REG_ERROR_TIMEOUT;
                emit OperationResult(cmd);
            }
        }
        m_cmdlist.pop_front();
    }
    QTimer::singleShot(m_interval,this,SLOT(doWork()));
}

bool RtuReader::addCmd(RegOperCmd cmd)
{
    if(m_cmdlist.size() > 5)
        return false;
    m_cmdlist.push_back(cmd);
    return true;
}
