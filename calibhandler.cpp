#include "calibhandler.h"

#include "modbus.h"
#include "wcommon.h"
#include <qdebug.h>


CalibHandler::CalibHandler(RTU_Modbus *rtu):
    CmdHandler(rtu),
    m_sensor_num(0)
{
    for(int i = 0 ;i < 6; i++)
    {
        m_set_calib_points[i] = false;
        m_read_calib_points[i] = false;
    }
}

bool CalibHandler::readRtParas(int num)
{
    quint16 values[24];
    if(num == 0)
        return false;
    bool ok = true;
    //read weights
    if(_rtu->read_registers(REG_SENSOR_WT,num*2,values) == num*2)
    {
        for(int i = 0; i < num; i++)
        {
            int weight = values[0] + (values[1] << 16);
            sensors[i].wt = weight;
        }

    }
    else
    {
        return false;
    }
    //read mv [uv*1000]
    if(_rtu->read_registers(REG_SENSOR_MV,num*2,values) == num*2)
    {
        for(int i = 0; i < num; i++)
        {
            int ad = values[0] + (values[1] << 16);
            sensors[i].mv = ad;
        }
    }else
    {
        return false;
    }


//    if(_rtu->read_registers(REG_SENSOR_Z,num*2,values) == num*2)
//    {
//        for(int i = 0; i < num; i++)
//        {
//            int z = values[0] + (values[1] << 16);
//            sensors[i].zero = z;
//        }
//    }else
//    {
//        return false;
//    }
    emit calibReadResult(sensors,num);
    return true;
}
bool CalibHandler::readSensorNum()
{
    if(m_sensor_num == 0)
    {
        quint16 result = 0;
        if(1 == _rtu->read_registers(REG_SENOSR_NUM,1,&result))
        {
           if(result < 1 && result > MAX_SENSOR_NUM)
           {
               return false;
           }
           m_sensor_num = result;
        }
    }
    return true;

}
bool CalibHandler::run()
{
    if(_rtu)
    {
        if(!readSensorNum())
        {
            return true;
        }
        readRtParas(m_sensor_num);
        for(int i = 0; i < m_sensor_num; i++)
        {

            if(m_read_calib_points[i])
            {
                quint16 values[16];
                if(_rtu->read_registers(REG_SENSOR_K+i*2,2,values) == 2)
                {

                    int k = values[0] + (values[1] << 16);
                    sensors[i].k = k;
                    sensors[i].valid = true;
                    m_read_calib_points[i] = false;
                    emit calibProcessResult(i,k);
                }

            }
            //read realtime params

        }



    }
    return true;
}
//实时读取mv,ad,weight.k值
//标定零点和重量设置.
bool CalibHandler::readPara(int index)
{
    if(index == -1)
    {
        for(int i = 0 ;i < 6; i++)
        {
            m_read_calib_points[i] = true;
            sensors[i].valid = false;
        }

    }else
    {
        m_read_calib_points[index] = true;
        sensors[index].valid = false;
    }
    m_sensor_num = 0;
    return true;
}

bool CalibHandler::stop()
{
    for(int i = 0 ;i < 6; i++)
    {
        m_set_calib_points[i] = false;
        m_read_calib_points[i] = false;
    }
    m_sensor_num = 0;
    return true;
}

bool CalibHandler::calibSet(bool hand,int index, qint32 weight, qint32 ad)
{
    quint16 values[4];

    values[0] = weight&0xFFFF;
    values[1] = (weight>>16)&0xFFFF;

    if(2 == _rtu->write_registers(REG_INPUT_WTS+index*2,2,values))
    {
        m_set_calib_points[index] = true;
        return true;
    }
    return false;
}

bool CalibHandler::calibSetAll(std::vector<int> weights,bool hand)
{
    quint16 values[32];
    int num = weights.size();
    for(int i = 0; i < num ;i++)
    {
        values[i*2]   = weights[i]&0xff;
        values[i*2+1] = (weights[i]>>16)&0xff;
    }
    if(num*2 == _rtu->write_registers(REG_INPUT_WTS,num*2,values))
    {
        //m_set_calib_points[index] = true;
        return true;
    }
    return false;
}

bool CalibHandler::calibZeroSet(int index)
{
    quint16 values[1];
    if(index > 0)
    {
        values[0] = 0;
        values[0] |=  (1 << index);
    }


    if(1 == _rtu->write_registers(REG_CALC_ZERO,1,values))
    {
        //m_set_calib_points[index] = true;
        return true;
    }
    return true;
}

bool CalibHandler::calibAllZero(int num)
{
    quint16 values[1];
    values[0] = 0;
    for(int i =0; i < num; i++)
    {
        values[0] |=  (1 << i);
    }
    if(1 == _rtu->write_registers(REG_CALC_ZERO,1,values))
    {
        //m_set_calib_points[index] = true;
        return true;
    }
}

bool CalibHandler::modifyKs(std::vector<qint32> ks)
{
    quint16 values[32];
    int num = ks.size();
    for(int i = 0; i < num ;i++)
    {
        values[i*2]   = ks[i]&0xffff;
        values[i*2+1] = (ks[i]>>16)&0xffff;
    }
    if(num*2 == _rtu->write_registers(REG_SENSOR_K,num*2,values))
    {
        //m_set_calib_points[index] = true;
        return true;
    }
    return false;
}
