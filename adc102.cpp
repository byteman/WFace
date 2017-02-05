#include "adc102.h"
#include "scanhandler.h"
#include "weighthandler.h"
#include "calibhandler.h"
#include "updatehandler.h"
#include <qdebug>
ADC102::ADC102(QObject *parent) : QObject(parent),
    m_handler(NULL),
    m_interval(100),
    m_slaveAddr(32),
    m_connect(false)
{
    ScanHandler* handler_scan = new ScanHandler(&modbus);
    connect(handler_scan,SIGNAL(scanResult(int,int)),this,SLOT(onScanResult(int,int)));
    WeightHandler* handler_weight = new WeightHandler(&modbus);
    connect(handler_weight,SIGNAL(weightResult(int,quint16,quint16,qint32,qint32)),this,SLOT(onWeightResult(int,quint16,quint16,qint32,qint32)));
    ParaHandler* handler_para = new ParaHandler(&modbus);
    connect(handler_para,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    CalibHandler* handler_calib = new CalibHandler(&modbus);
    connect(handler_calib,SIGNAL(calibProcessResult(int,int)),this,SLOT(onCalibProcessResult(int,int)));
    connect(handler_calib,SIGNAL(calibReadResult(int,qint32,qint32)),this,SLOT(onCalibPointResult(int,int,int)));
    handler_update = new UpdateHandler(&modbus);
    connect(handler_update,SIGNAL(updateResult(int,int,int)),this,SLOT(onUpdateResult(int,int,int)));
    m_handlers.push_back(handler_scan);
    m_handlers.push_back(handler_weight);
    m_handlers.push_back(handler_para);
    m_handlers.push_back(handler_calib);
    //m_handlers.push_back(handler_update);
}

bool ADC102::setSlaveAddr(int addr)
{
    bool ret = true;
    m_slaveAddr = addr;

    modbus.setDeviceAddr(addr);
    m_connect = ret;
    return m_connect;
}

bool ADC102::hasConnect()
{
    return m_connect;
}

bool ADC102::discardTare()
{
    return modbus.write_register(2,2)==1?true:false;

}

bool ADC102::setZero()
{
    return modbus.write_register(2,1)==1?true:false;
}

bool ADC102::zoom10X()
{
    return modbus.write_register(2,4)==1?true:false;
}

bool ADC102::changeGN()
{
    return modbus.write_register(2,5)==1?true:false;
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

bool ADC102::startScan(QString port, int baud, char parity, char databit, char stopbit,bool findOne)
{
    if(m_handler!=NULL)
    {
        m_handler->stop();
    }
    if(!modbus.open(port.toStdString().c_str(),baud,parity,databit,stopbit))
    {

        return false;
    }
    modbus.setByteTimeout(100000);
    modbus.set_response_timeout(100000);
    ScanHandler* handler = (ScanHandler*)m_handlers[0];
    handler->startScan(findOne);
    //m_handlers.push_back();
    m_handler = handler;
    m_interval = 100;
    QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
    return true;
}
bool ADC102::stopScan()
{
    ScanHandler* handler = (ScanHandler*)m_handlers[0];
    return handler->stop();
}
bool ADC102::startReadWeight()
{
    if(m_handler!=NULL)
    {
        m_handler->stop();
    }
    //m_handlers.push_back();
    ParaHandler* handler = (ParaHandler*)m_handlers[2];
    if(handler!=NULL)
    {

        if(!handler->paraRead(m_para))
        {
            return false;
        }
    }
    m_handler = m_handlers[1];
    m_interval = 100;
    QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
    return true;
}

bool ADC102::startReadPara()
{
    if(m_handler!=NULL)
    {
        m_handler->stop();
    }
    m_handler = m_handlers[2];
    ParaHandler* handler = (ParaHandler*)m_handler;
    if(handler!=NULL)
    {
        handler->start();
    }
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

bool ADC102::startCalib(int index, int weight)
{

    CalibHandler* handler = (CalibHandler*)m_handlers[3];
    return handler->calibSet(index,weight,0);

}

bool ADC102::readCalibPoints(int index)
{

    if(m_handler!=NULL)
    {
        m_handler->stop();
    }
    CalibHandler* handler = (CalibHandler*)m_handlers[3];
    m_handler = m_handlers[3];
    handler->readPara(index);
    m_interval = 500;
    QTimer::singleShot(m_interval,this,SLOT(timerHandler()));
    return true;
}

bool ADC102::stopReadWeight()
{
    return true;
}

bool ADC102::stop()
{
    return true;
}

void ADC102::onCalibPointResult(int index, int weight, int ad)
{
    emit calibPointResult(index,weight,ad);
}

void ADC102::onParaReadResult(Para _para)
{
    emit paraReadResult(_para);
}
void ADC102::onCalibProcessResult(int index, int result)
{
    emit calibProcessResult(index,result);
}
void ADC102::onScanResult(int type, int addr)
{
    emit scanResult(type,addr);
}

void ADC102::onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare)
{
    emit weightResult(weight,state,dot,gross,tare);
}

void ADC102::onUpdateResult(int result, int pos, int total)
{
    emit updateResult(result,pos,total);
}

void ADC102::timerHandler()
{


}

bool ADC102::modifyAddr(quint16 oldAddr, quint16 newAddr)
{
    modbus.setDeviceAddr(oldAddr);
    if(1  == modbus.write_register(30,newAddr) )
    {
        //这里修改了地址后，设备就是以新的地址返回数据，所以这里判断返回值就不对了，让用户直接去搜索，不用管返回值了.
        //return true;
    }
    return true;
}

bool ADC102::reset()
{

    return modbus.write_register(2,99)==1?true:false;
    //modbus.write_register(2,99);
    //return true;
}

bool ADC102::startUpdate(QString file)
{
    if(m_handler!=NULL)
    {
        m_handler->stop();
    }
    m_handler = NULL;
    //m_handlers.push_back();


    if(handler_update!=NULL)
    {

        modbus.close();
        if(!handler_update->startUpdate(file))
        {
            return false;
        }
    }

    return true;
}

