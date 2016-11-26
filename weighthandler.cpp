#include "weighthandler.h"
#include "modbus.h"
#include "wcommon.h"
#include <qdebug.h>

WeightHandler::WeightHandler(RTU_Modbus *rtu):
    CmdHandler(rtu)
{

}


bool WeightHandler::run()
{
    if(_rtu)
    {
        quint16 values[8];
        if(4 == _rtu->read_registers(REG_RT_WT,4,values))
        {
            int weight = values[0] + (values[1]<<16);
            emit weightResult(weight,values[2],values[3],values[4]+(values[5]<<16),values[6] +( values[7]<<16 ) );
        }
    }
    return true;
}

bool WeightHandler::stop()
{
    return true;
}
