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


bool CalibHandler::run()
{
    if(_rtu)
    {
        for(int i = 0; i < 6; i++)
        {
            if(m_set_calib_points[i])
            {


                if(3 == _rtu->write_registers(20,3,values))
            }
            if(m_read_calib_points[i])
            {


            }
        }

    }
    return true;
}

bool CalibHandler::calibSet(int index, qint32 weight, qint32 ad)
{
    quint16 values[4];
    values[0] = index;
    values[1] = weight&0xFFFF;
    values[2] = (weight>>16)&0xFFFF;

    if(3 == _rtu->write_registers(20,3,values))
    {
        m_set_calib_points[i] = true;
    }
}
