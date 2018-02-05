#include "calibhandler.h"

#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>


CalibHandler::CalibHandler(RtuReader *rtu):
    CmdHandler(rtu)
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

    if(4 == _rtu->read_registers(REG_FULL_SPAN,4,values))
    {
        quint32 sensor_full_span = values[0]+(values[1]<<16);
        quint32 sensor_mv = values[2]+(values[3]<<16);
        emit calibParaResult(sensor_mv, sensor_full_span);
        return true;
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
                        emit calibReadResult(index, weight,ad);
                        m_read_calib_points[i] = false;
                    }
                }

            }
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

bool CalibHandler::calibSet(int index, qint32 weight, qint32 ad)
{
    quint16 values[4];
    values[0] = index;
    values[1] = 1;
    values[2] = weight&0xFFFF;
    values[3] = (weight>>16)&0xFFFF;
    m_set_calib_points[index] = true;
    return postWriteRegs(REG_CALIB, 4, values);
}

bool CalibHandler::savePara( quint32 full,quint32 mv)
{
    quint16 values[4];
    values[0] = (full&0xFFFF);
    values[1] = (full>>16)&0xFFFF;
    values[2] = (mv&0xFFFF);
    values[3] = (mv>>16)&0xFFFF;
    return postWriteRegs(REG_FULL_SPAN, 4, values);
}
