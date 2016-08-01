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
       // _rtu->write_registers();
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
            m_ok = true;
            emit paraReadResult(m_para);
        }
    }

    return !m_ok;
}
