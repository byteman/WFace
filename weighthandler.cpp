#include "weighthandler.h"
#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>

WeightHandler::WeightHandler(ModbusReader *rtu):
    CmdHandler(rtu)
{

}
bool WeightHandler::paraRead()
{
    quint16 values[6];
    if(6 == _rtu->read_registers(REG_DIV_HIGH,6,values))
    {

        quint16 div_high = values[0];
        quint16 div_low = values[1];
        quint32 span_high = values[2]+(values[3]<<16);
        quint32 span_low = values[4]+(values[5]<<16);
        if(1 == _rtu->read_registers(REG_DOT,1,values))
        {
            m_dot = values[0];
            emit weightParaReadResult(div_high,div_low, span_high,span_low,m_dot);
            return true;
        }

    }
    return false;
}
bool WeightHandler::doWork()
{
    qDebug() << "Weight doWork";
    if(_rtu)
    {
        quint16 values[8];
        if(4 == _rtu->read_registers(0,4,values))
        {
            emit weightResult(values[0]+(values[1]<<16),values[2],values[3],values[4]+(values[5]<<16),values[6] +( values[7]<<16 ) );
        }

        if(!bInit)
        {
           bInit = paraRead();
        }

        this->msleep(100);
        //qDebug() << "weight read";
        return false;
    }
    return true;
}
bool WeightHandler::WriteCtrlCmd(int reg, quint8 value)
{

    RegCmd cmd;
    cmd.reg_addr = reg;
    cmd.reg_num = 1;
    cmd.reg_value[0] = value;
    return postCmd(cmd);
}


bool WeightHandler::discardTare()
{
    return WriteCtrlCmd(2,2);

}

bool WeightHandler::setZero()
{

    return WriteCtrlCmd(2,1);

}

bool WeightHandler::zoom10X()
{
   return WriteCtrlCmd(2,4);
}

bool WeightHandler::changeGN()
{
    return WriteCtrlCmd(2,5);
}

int WeightHandler::getDot()
{
    return m_dot;
}


