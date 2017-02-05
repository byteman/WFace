#include "weighthandler.h"
#include "modbus.h"

#include <qdebug.h>

WeightHandler::WeightHandler(RtuReader *rtu):
    CmdHandler(rtu)
{

}

void WeightHandler::run()
{
    if(_rtu)
    {
        quint16 values[8];
        if(4 == _rtu->read_registers(0,4,values))
        {
            emit weightResult(values[0]+(values[1]<<16),values[2],values[3],values[4]+(values[5]<<16),values[6] +( values[7]<<16 ) );
        }
    }
    writeCmds();
    return ;
}

bool WeightHandler::stop()
{
    return true;
}


