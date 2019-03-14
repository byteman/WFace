#include "pollhandler.h"
#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>
#include "command.h"
PollerHandler::PollerHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_start(0),
    m_end(0),
    m_cur_addr(0),
    m_start_us(30000),
    m_stop_us(1000000),
    m_read_delay_ms(10),
    m_show_ad(false),
    m_num(0),
    m_quit(false)
{

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
#include <vector>
bool PollerHandler::readWgt()
{
    QByteArray outArr;
    quint8 cmd = m_show_ad?CMD_CUSTOM_READ_AD:CMD_READ_WGT;

    int result = _rtu->send_then_recv(cmd,QByteArray(),outArr,256);
    if(result > 0)
    {
        quint8 sensor_id = outArr[0];
        quint8 sensor_num= outArr[1];
        std::vector<qint32> sums;
        outArr.remove(0,2);
        quint8 num = outArr.size() / 4;
        qint32 sum = 0;

        int group = 0;
        int grpNum = 1;
        if(m_num > 0){
           group = (num+m_num-1) / m_num;
        }
        qDebug() << "group=" <<group << "grpNum=" <<grpNum;
        if(num ==0){
            return true;
        }
        bool add = false;
        for(int i= 0; i < num; i++)
        {
            quint8  addr   = outArr[i*4+0];
            quint8  state  = outArr[i*4+1];
            quint8 w1 = outArr[i*4+2];
            quint8 w2 = outArr[i*4+3];

            qint16 value  = (w1<<8) + w2;
            quint16 ad = value;
            if(m_show_ad){
                state |= 0x80;

                emit weightResult(addr,ad,state,0,0,0 );

            }else{
                state &= 0x7F;
                if(m_num > 0){
                    if(i < m_num*grpNum){
                        add = true;
                        sum+=value;
                    }else{
                        sums.push_back(sum);
                        sum = 0;
                        sum+=value;
                        grpNum++;
                        add = false;
                    }
                }

                emit weightResult(addr,value,state,0,0,0 );
            }


        }
        if(!m_show_ad & m_num>0){
            if(add){
                sums.push_back(sum);

            }
            qint32 result[4]={0,};
            for(int i = 0; i < sums.size() && i < 4; i++){
                result[i] = sums[i];
            }
            weightSumResult(m_num, result[0], result[1], result[2], result[3]);

        }
    }
    else
    {
        qDebug() << "timeout";
        emit timeout(0);
    }

    return true;
}
bool PollerHandler::doWork()
{
    //qDebug() << "PollerHandler doWork";
    if(_rtu)
    {

        if(!canRead())
        {
            msleep(1);
            return false;
        }
        _rtu->setDeviceAddr(0);

        if(readWgt())
        {

        }

        if(m_quit)
        {
            qDebug() << "PollerHandler quit";
            return true;
        }
        msleep(100);
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
    //_rtu->set_response_timeout(startUs);
}

void PollerHandler::setReadInterval(int ms)
{
    m_read_delay_ms = ms;
}

void PollerHandler::showAD(bool en)
{
    m_show_ad = en;
}

void PollerHandler::setSumNum(int num)
{
    m_num = num;
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
    //_rtu->set_response_timeout(m_stop_us);
    return true;
}
