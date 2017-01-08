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
       if(14 == _rtu->write_registers(21,14,values))
       {
            return true;
       }



    }
    return false;
}

bool ParaHandler::paraRead(Para &_para)
{
    quint16 values[72];
//    if(17 == _rtu->read_registers(3,17,values))
//    {

        //m_para.dot = values[0];
//        _para.dot = values[0];
//        _para.div_high = values[5];
//        _para.div_low = values[6];
//        _para.span_high = values[7]+(values[8]<<16);
//        _para.span_low = values[9]+(values[10]<<16);
//        _para.unit = values[11];
//        _para.pwr_zero_span = values[12];
//        _para.hand_zero_span = values[13];
//        _para.zero_track_span = values[14];
//        _para.stable_span = values[15];
//        _para.filter_level = values[16];
//        //memcpy(&m_para,&values[0],sizeof(values));
//        if(6 == _rtu->read_registers(26,6,values))
//        {
//            _para.sensor_full_span = values[0]+(values[1]<<16);
//            _para.sensor_mv = values[2]+(values[3]<<16);
//            _para.slave_addr = values[4];
//            _para.version = values[5];
//            if(1 == _rtu->read_registers(96,1,&_para.adRate))
//            {
//                emit paraReadResult(_para);
//                return true;
//            }

//        }


    //}

    if(36 == _rtu->read_registers(1,36,values))
    {
        _para.serverPort = values[20];
        _para.serverIp =values[21] + (values[22] << 16);
        memcpy(_para.plate,values+23,10);
        memcpy(_para.simCard,values+28,12);
        _para.sensorNum = values[7];
        _para.div_high = values[8];
        _para.max_weight = values[0] + (values[1] << 16);
        emit paraReadResult(_para);
        return true;
    }
    return false;
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
