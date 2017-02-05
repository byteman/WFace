#include "calibhandler.h"

#include "modbus.h"

#include <qdebug.h>


CalibHandler::CalibHandler(RTU_Modbus *rtu):
    CmdHandler(rtu)
{
    for(int i = 0 ;i < 6; i++)
    {
        m_set_calib_points[i] = false;
        m_read_calib_points[i] = false;
    }
}


void CalibHandler::run()
{
    if(_rtu)
    {
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

    }

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

bool CalibHandler::stop()
{
    for(int i = 0 ;i < 6; i++)
    {
        m_set_calib_points[i] = false;
        m_read_calib_points[i] = false;
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

    if(4 == _rtu->write_registers(20,4,values))
    {
        m_set_calib_points[index] = true;
        return true;
    }
    return false;
}
