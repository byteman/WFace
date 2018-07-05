#include "calibhandler.h"

#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>


CalibHandler::CalibHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_dot(0),
    m_sensor(0)
{
    for(int i = 0 ;i < 6; i++)
    {
        m_set_calib_points[i] = false;
        m_read_calib_points[i] = false;
    }
}

bool CalibHandler::paraRead(void)
{
    quint16 values[17];

    if(1 == _rtu->read_registers(REG_2B_SENSOR_NUM,1,values))
    {


        m_sensor = (values[0] > 8)?4:values[0];
        if(4 == _rtu->read_registers(REG_FULL_SPAN,4,values))
        {

            quint32 sensor_full_span = values[0]+(values[1]<<16);
            quint32 sensor_mv = values[2]+(values[3]<<16);
            emit calibParaResult(sensor_mv, sensor_full_span);

            if(1 == _rtu->read_registers(REG_DOT,1,values))
            {
                m_dot = values[0];
                return true;
            }
        }
    }


    return false;
}
bool CalibHandler::doWork()
{

    if(_rtu)
    {
        //qDebug() << "calib work";
        if(!bInit)
        {
            bInit = paraRead();
        }
        for(int i = 0; i < 6; i++)
        {
            if(m_set_calib_points[i])
            {  
                quint16 result = i;
                if(1 == _rtu->read_registers(21,1,&result))
                {

                    if(result <= 0)
                    {
                        //calib complete
                        m_set_calib_points[i] = false;
                    }
                    calibProcessResult(i,result);
                }
            }
            if(m_read_calib_points[i])
            {
                quint16 index = i;
                if(_rtu->write_registers(20,1,&index) == 1)
                {

                    quint16 values[4];
                    if(_rtu->read_registers(22,4,values) == 4)
                    {

                        int weight = values[0] + (values[1] << 16);
                        int ad = values[2] + (values[3] << 16);
                        emit calibReadResult(index, weight,ad,m_dot);
                        m_read_calib_points[i] = false;
                    }
                }

            }
        }
        quint16 values[17];
        if(m_sensor*2 == _rtu->read_registers(REG_4B_CHANNEL_AD,m_sensor*2,values))
        {
            //定时读取各路通道AD值.

            QList<float> chanAD;
            for(int i = 0; i < m_sensor; i++)
            {
                chanAD.push_back(values[i*2]+(values[i*2+1]<<16));
            }
            emit chanADReadResult(chanAD);

        }
        this->msleep(100);
        return false;
    }

    return true;


}

bool CalibHandler::init()
{
    return readPara();
}

bool CalibHandler::readPara(int index)
{
    if(index == -1)
    {
        for(int i = 0 ;i < 6; i++)
        {
            m_read_calib_points[i] = true;
        }

    }else
    {
        m_read_calib_points[index] = true;
    }
    return true;
}

bool CalibHandler::calibSet(int index, float weight, qint32 ad)
{
    if(!bInit) return false;
    quint16 values[4];
    values[0] = index;
    values[1] = 1;
    quint32 iwet = weight * pow(10,m_dot);
    values[2] = iwet&0xFFFF;
    values[3] = (iwet>>16)&0xFFFF;
    m_set_calib_points[index] = true;
    return postWriteRegs(REG_CALIB, 4, values);
}

bool CalibHandler::savePara( quint32 full,quint32 mv)
{
    if(!bInit) return false;
    quint16 values[4];
    values[0] = (full&0xFFFF);
    values[1] = (full>>16)&0xFFFF;
    values[2] = (mv&0xFFFF);
    values[3] = (mv>>16)&0xFFFF;
    return postWriteRegs(REG_FULL_SPAN, 4, values);
}
