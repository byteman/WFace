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
        values[0] = _para.dot;
        values[1] = _para.div_high;
        values[2] = _para.div_low;
        values[3] = (_para.span_high&0xFFFF);
        values[4] = (_para.span_high>>16)&0xFFFF;
        values[5] = (_para.span_low&0xFFFF);
        values[6] = (_para.span_low>>16)&0xFFFF;
        values[7] = _para.unit;
        values[8] = _para.pwr_zero_span;
        values[9] = _para.hand_zero_span;
        values[10] = _para.zero_track_span;
        values[11] = _para.stable_span;
        values[12] = _para.filter_level;

       if(1 == _rtu->write_registers(3,1,values))
       {
           if(_rtu->write_registers(8,2,values+1) != 2)return false;
           if(_rtu->write_registers(10,2,values+3) != 2)return false;
           if(_rtu->write_registers(12,2,values+5) != 2)return false;
           if(_rtu->write_registers(14,6,values+7) != 6)return false;


           values[0] = (_para.sensor_full_span&0xFFFF);
           values[1] = (_para.sensor_full_span>>16)&0xFFFF;
           values[2] = (_para.sensor_mv&0xFFFF);
           values[3] = (_para.sensor_mv>>16)&0xFFFF;
           values[4] = _para.slave_addr;
           if(5 == _rtu->write_registers(26,5,values))
           {
               return true;
           }

       }

    }
    return false;
}

bool ParaHandler::paraRead(Para &_para)
{
    quint16 values[17];
    if(17 == _rtu->read_registers(3,17,values))
    {

        //m_para.dot = values[0];
        _para.dot = values[0];
        _para.div_high = values[5];
        _para.div_low = values[6];
        _para.span_high = values[7]+(values[8]<<16);
        _para.span_low = values[9]+(values[10]<<16);
        _para.unit = values[11];
        _para.pwr_zero_span = values[12];
        _para.hand_zero_span = values[13];
        _para.zero_track_span = values[14];
        _para.stable_span = values[15];
        _para.filter_level = values[16];
        //memcpy(&m_para,&values[0],sizeof(values));
        if(6 == _rtu->read_registers(26,6,values))
        {
            _para.sensor_full_span = values[0]+(values[1]<<16);
            _para.sensor_mv = values[2]+(values[3]<<16);
            _para.slave_addr = values[4];
            _para.version = values[5];
            emit paraReadResult(_para);
            return true;
        }

    }
    return false;
}

bool ParaHandler::run()
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
