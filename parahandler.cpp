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
        if(2 == _rtu->read_registers(3,17,values))
        {

            //m_para.dot = values[0];

            memcpy(&m_para,&values[0],sizeof(values));
            m_ok = true;
            emit paraReadResult(m_para);
        }
    }

    return !m_ok;
}
