#include "cornhandler.h"

#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>


CornHandler::CornHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_sensor(0)
{

}
//读取角差系数
bool CornHandler::paraRead(void)
{

    quint16 values[17];

    if(1 == _rtu->read_registers(REG_2B_SENSOR_NUM,1,values))
    {
        m_sensor = values[0];
        if(8 == _rtu->read_registers(REG_4B_CORN_K,8,values))
        {
            QList<float> chanK;
            for(int i = 0; i < 4; i++)
            {
                //values[7]+(values[8]<<16);
                float tmp = (values[i*2+0]+(values[i*2+1]<<16));
                float k = (float)tmp/1000.0f;
                chanK.push_back(k);
            }

            emit chanKReadResult(m_sensor,chanK);
            return true;
        }
    }



    return false;
}

bool CornHandler::setK(int index,float k)
{
    qint32 ik = k*1000;
    quint16 values[2];
    values[0] = ik&0xffff;
    values[1] = (ik>>16)&0xffff;
    return postWriteRegs(REG_4B_CORN_K+index*2,2,values);
}

bool CornHandler::setKs(QList<float> ks)
{

    quint16 values[8];
    if(ks.size() != 4) return false;
    for(int i = 0; i < ks.size(); i++)
    {
        qint32 ik = ks[i]*1000;
        values[i*2+0] = ik&0xffff;
        values[i*2+1] = (ik>>16)&0xffff;
    }

    return postWriteRegs(REG_4B_CORN_K,8,values);
}

bool CornHandler::setSensorNum(quint16 num)
{
    return postWriteRegs(REG_2B_SENSOR_NUM ,1 ,&num);
}

void CornHandler::updateSensorNum(quint16 num)
{
    m_sensor = num;
}

int CornHandler::getSensorNum()
{
    return m_sensor;
}
bool CornHandler::doWork()
{

    qDebug() << "CornHandler doWork";
    if(_rtu)
    {
        //qDebug() << "calib work";
        if(!bInit)
        {
            bInit = paraRead();
        }
        quint16 values[8];
        if(8 == _rtu->read_registers(REG_4B_CHANNEL_AD,8,values))
        {
            //定时读取各路通道AD值.
            QList<float> chanAD;
            chanAD.push_back(values[0]+(values[1]<<16));
            chanAD.push_back(values[2]+(values[3]<<16));
            chanAD.push_back(values[4]+(values[5]<<16));
            chanAD.push_back(values[6]+(values[7]<<16));

            emit chanADReadResult(chanAD);

        }
        this->msleep(200);
        return false;
    }

    return true;


}

bool CornHandler::init()
{
    return true;
}

bool CornHandler::startCalib()
{
    quint16 value = 0;

    return postWriteRegs(REG_2B_AUTO_CORN, 1, &value);
}

bool CornHandler::calib(int index)
{
    if( (index < 1) || (index > 4)) return false;
    quint16 value = index;

    return postWriteRegs(REG_2B_AUTO_CORN, 1, &value);
}

bool CornHandler::ReadParam()
{
    bInit = false;
    return bInit;
}
bool CornHandler::stopCalib()
{
    quint16 value = 5;

    return postWriteRegs(REG_2B_AUTO_CORN, 1, &value);
}
