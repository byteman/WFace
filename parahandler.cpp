#include "parahandler.h"

ParaHandler::ParaHandler(RTU_Modbus *rtu):
    CmdHandler(rtu),
    m_ok(false)
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


       if(13 == _rtu->write_registers(3,13,values))
       {
           values[0] = (_para.sensor_full_span&0xFFFF);
           values[1] = (_para.sensor_full_span>>16)&0xFFFF;
           values[2] = (_para.sensor_mv&0xFFFF);
           values[3] = (_para.sensor_mv>>16)&0xFFFF;
           values[4] = _para.slave_addr;
           if(13 == _rtu->write_registers(3,13,values))
           {
               return true;
           }
       }
    }
    return false;
}

bool ParaHandler::run()
{
    if(_rtu && !m_ok)
    {
        quint16 values[17];
        if(17 == _rtu->read_registers(3,17,values))
        {

            //m_para.dot = values[0];
            m_para.dot = values[0];
            m_para.div_high = values[5];
            m_para.div_low = values[6];
            m_para.span_high = values[7]+(values[8]<<16);
            m_para.span_low = values[9]+(values[10]<<16);
            m_para.unit = values[11];
            m_para.pwr_zero_span = values[12];
            m_para.hand_zero_span = values[13];
            m_para.zero_track_span = values[14];
            m_para.stable_span = values[15];
            m_para.filter_level = values[16];
            //memcpy(&m_para,&values[0],sizeof(values));
            if(6 == _rtu->read_registers(26,6,values))
            {
                m_para.sensor_full_span = values[0]+(values[1]<<16);
                m_para.sensor_mv = values[2]+(values[3]<<16);
                m_para.slave_addr = values[4];
                m_para.version = values[5];
                m_ok = true;
                emit paraReadResult(m_para);
            }

        }

    }

    return !m_ok;
}
