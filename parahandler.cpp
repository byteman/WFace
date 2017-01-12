#include "parahandler.h"

ParaHandler::ParaHandler(RTU_Modbus *rtu):
    CmdHandler(rtu),
    m_ok(true)
{

}

void ParaHandler::start()
{
    m_ok = false;
}

bool ParaHandler::stop()
{
    m_ok = true;
    return m_ok;
}

bool ParaHandler::paraSave(Para _para)
{
    if(_rtu)
    {
       quint16 values[17];

       values[0] = _para.max_weight&0xFFFF;
       values[1] = (_para.max_weight>>16)&0xFFFF;
       values[2] = _para.max_weight&0xFFFF;
       values[3] = (_para.max_weight>>16)&0xFFFF;

       if(4 != _rtu->write_registers(1,4,values))
       {
            return false;
       }

       values[0] = _para.sensorNum;
       values[1] = _para.div_high;
       if(2 != _rtu->write_registers(8,2,values))
       {
            return false;
       }
       values[0] = _para.serverPort;
       values[1] = (_para.serverIp)&0xFFFF;

       values[2] = (_para.serverIp>>16)&0xFFFF;

       memcpy(values+3,_para.plate,10);
       memcpy(values+8,_para.simCard,12);
       if(14 != _rtu->write_registers(21,14,values))
       {
            return false;
       }
       values[0] = _para.gps_report;
       values[1] = _para.dev_report;   

       if(2 != _rtu->write_registers(164,2,values))
       {
            return false;
       }
       values[0] = _para.delay_time;
       if(1 == _rtu->write_registers(169,1,values))
       {
            return true;
       }
    }
    return false;
}

bool ParaHandler::paraRead(Para &_para)
{
    quint16 values[72];

    if(36 != _rtu->read_registers(1,36,values))
    {
        return false;
    }
    _para.serverPort = values[20];
    _para.serverIp =values[21] + (values[22] << 16);
    memcpy(_para.plate,values+23,10);
    memcpy(_para.simCard,values+28,12);
    _para.sensorNum = values[7];
    _para.div_high = values[8];
    _para.max_weight = values[0] + (values[1] << 16);
    if(11 != _rtu->read_registers(159,11,values)){
        return false;
    }
    _para.version =  values[0]  + (values[1] << 16);
    _para.gps_beidou = values[4];
    _para.gps_report = values[5];
    _para.dev_report = values[6];
    memcpy(_para.calib_datetime,values+7,6);
    _para.delay_time = values[10];
    emit paraReadResult(_para);
    return true;

}

bool ParaHandler::myrun()
{
    if(_rtu && !m_ok)
    {
       if(paraRead(m_para))
       {
           m_ok = true;

       }
    }

    return !m_ok;
}
