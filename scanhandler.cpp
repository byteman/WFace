#include "scanhandler.h"

#include <qdebug.h>

ScanHandler::ScanHandler(ModbusReader *rtu):
    CmdHandler(rtu),
    m_addr(1),
    m_start_us(100000),
    m_stop_us(1000000),
    m_findOnce(true),
    m_find_done(false),
    m_quit(false)
{
    m_addrArr.clear();
}

QVector<int> ScanHandler::getAddrList()
{
    return m_addrArr;
}
void ScanHandler::setTimeOut(int startUs, int stopUs)
{
    m_start_us = startUs;
    m_stop_us = stopUs;
}
bool ScanHandler::init(int reg_addr,int reg_size,int min_addr,int max_addr,bool findOne)
{
    m_addr = 1;
    m_find_done = false;
    m_findOnce = findOne;
    m_reg_addr = reg_addr;
    m_reg_size = reg_size;
    m_end_addr = max_addr;
    m_start_addr = min_addr;
    m_quit = false;
    m_addrArr.clear();
    _rtu->set_response_timeout(m_start_us);
    return true;
}

bool ScanHandler::poll()
{
    qDebug() << "poll";
    if(_rtu && m_addrArr.size() > 0)
    {
        if( m_cur_index < m_addrArr.size() && m_cur_index>=0)
        {
            quint16 values[8];
            m_cur_addr = m_addrArr[m_cur_index];

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

//        if(m_quit)
//        {
//            m_cur_index = 0;
//            qDebug() << "ScanHandler quit";
//            return true;
//        }
        return false;
    }
    return true;

}
bool ScanHandler::scan()
{
    qDebug() << "scan";
    if(m_addr < m_end_addr )
    {
        emit scanResult(SCAN_PROGRASS,m_addr);
        if(_rtu)
        {
            quint16 state = 0;

            _rtu->setDeviceAddr(m_addr);
            int len = _rtu->read_registers(m_reg_addr,m_reg_size,&state);
            _rtu->getCurrentDeviceAddr();
            qDebug() << "salve addr=" << m_addr << " reg_addr=" << m_reg_addr << "value=" << state;
            if(len == m_reg_size && state==_rtu->getCurrentDeviceAddr())
            {
                emit scanResult(SCAN_FIND,m_addr);
                m_addrArr.push_back(m_addr);
                if(m_findOnce)
                {
                    m_addr = m_start_addr;
                    emit scanResult(SCAN_COMPLETE,m_addr);
                    return true;
                }

            }
            _rtu->flush();
        }

        m_addr++;
        msleep(10);
        //还没有扫描完毕
        return false;
    }

    m_addr = m_start_addr;
    //扫描完毕.
    emit scanResult(SCAN_COMPLETE,m_addr);
    return true;
}
bool ScanHandler::SetQuit()
{
    qDebug() << "Quit- ";
    m_quit = true;
    return true;
}
bool ScanHandler::setZero(int addr)
{
    mutex.lock();
    m_clear_addr.push_back(addr);
    mutex.unlock();
    return true;
}
void ScanHandler::clear()
{
    int addr = 0;
    bool need = false;
    mutex.lock();
    if(m_clear_addr.size() > 0){
        addr = m_clear_addr.first();
        need = true;
        m_clear_addr.pop_front();
    }
    mutex.unlock();

    if(need && _rtu){
        quint16 v = 1;
        _rtu->setDeviceAddr(addr);
        _rtu->write_registers(2,1,&v);
    }
}
bool ScanHandler::doWork()
{
    qDebug() << "scan thread-id:" << QThread::currentThreadId();
    clear();
    if(!m_find_done)
    {
        //扫描状态就扫描.
        if(scan()){
            //扫描完成就进入
            //扫描完成,检查是否有数据
            if(m_addrArr.size() == 0){
                //没有扫描到设备就退出重量读取流程.
                //SetQuit();
                return true;
            }
            //可以继续流程
            m_find_done = true;
        }
        return false;
    }
    //扫描是一直在做，直到主动关闭。
    return poll();


}
//只是结束扫描，但是继续poll.
bool ScanHandler::stopScan(){
    m_find_done = true;
    m_addr = m_end_addr;
    emit scanResult(SCAN_COMPLETE,m_addr);
    _rtu->set_response_timeout(m_stop_us);
    return true;
}
//需要子类化停止函数,因为需要传递扫描完成的消息.
//这个停止是停止工作。
bool ScanHandler::stop()
{
    qDebug() <<"ScanHandler stop quit=" << m_quit;

    m_addr = m_end_addr;
    //SetQuit();

    //直接发送扫描完毕.等待线程结束.
    //this->wait();
    CmdHandler::stop();
    emit scanResult(SCAN_COMPLETE,m_addr);
    _rtu->set_response_timeout(m_stop_us);
    return true;
}
