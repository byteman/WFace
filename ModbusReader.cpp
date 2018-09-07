#include "ModbusReader.h"
#include <QDebug>



ModbusReader::ModbusReader(QObject *parent) : QObject(parent),
    m_connect(false),
    m_slaveAddr(-1)
{

}
//这个是接口,每种reader有自己的地址定义.
int ModbusReader::getCurrentDeviceAddr()
{
    return m_slaveAddr;
}

bool ModbusReader::start(int interval)
{
    m_interval = interval;
    m_thread.start();
    moveToThread(&m_thread);
    QTimer::singleShot(m_interval,this,SLOT(doWork()));
    return true;

}


void ModbusReader::doWork()
{

}
