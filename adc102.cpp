#include "adc102.h"
#include "scanhandler.h"
#include "weighthandler.h"
#include <qdebug>
ADC102::ADC102(QObject *parent) : QObject(parent),
    m_handler(NULL),
    m_interval(100),
    m_slaveAddr(32)
{
    ScanHandler* handler_scan = new ScanHandler(&modbus);
    connect(handler_scan,SIGNAL(scanResult(int,int)),this,SLOT(onScanResult(int,int)));
    WeightHandler* handler_weight = new WeightHandler(&modbus);
    connect(handler_weight,SIGNAL(weightResult(int,quint16)),this,SLOT(onWeightResult(int,quint16)));
    ParaHandler* handler_para = new ParaHandler(&modbus);
    connect(handler_para,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    m_handlers.push_back(handler_scan);
    m_handlers.push_back(handler_weight);
    m_handlers.push_back(handler_para);
}

void ADC102::setSlaveAddr(int addr)
{
    m_slaveAddr = addr;
    modbus.setDeviceAddr(addr);
}

bool ADC102::paraSave(Para _para)
{
    ParaHandler* handler = (ParaHandler*)m_handlers[2];
    if(handler)
    {
        return handler->paraSave(_para);
    }
    return false;
}

bool ADC102::startScan(QString port, int baud, char parity, char databit, char stopbit)
{
    if(!modbus.open(port.toStdString().c_str(),baud,parity,databit,stopbit))
    {

        return false;
    }

    //m_handlers.push_back();
    m_handler = m_handlers[0];
    m_interval = 100;
    QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
    return true;
}

bool ADC102::startReadWeight()
{

    //m_handlers.push_back();
    m_handler = m_handlers[1];
    m_interval = 100;
    QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
    return true;
}

bool ADC102::startReadPara()
{
    m_handler = m_handlers[2];
    m_interval = 1000;
    QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
    return true;
}

bool ADC102::stopReadPara()
{
    ParaHandler* handler = (ParaHandler*)m_handlers[2];
    if(handler)
    {
        return handler->stop();
    }
    return false;
}

bool ADC102::stopReadWeight()
{
    return true;
}

bool ADC102::stop()
{
    return true;
}

void ADC102::onParaReadResult(Para _para)
{
    emit paraReadResult(_para);
}

void ADC102::onScanResult(int type, int addr)
{
    emit scanResult(type,addr);
}

void ADC102::onWeightResult(int weight, quint16 state)
{
    emit weightResult(weight,state);
}

void ADC102::timerHandler()
{
    qDebug() <<    "adc102 timer";
    if(m_handler != NULL)
    {
        if(m_handler->run())
        {
            QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
        }
        else
        {
            m_handler = NULL;
        }

    }

}

