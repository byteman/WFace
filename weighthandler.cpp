#include "weighthandler.h"
#include "modbus.h"

#include <qdebug.h>

WeightHandler::WeightHandler(RTU_Modbus *rtu):
    CmdHandler(rtu)
{

}


bool WeightHandler::run()
{
    if(_rtu)
    {
        quint16 weight[3];
        if(2 == _rtu->read_registers(0,2,weight))
        {
            qint32 w = 0;
            memcpy(&w,&weight[0],sizeof(qint16)*2);
            emit weightResult(w,weight[2]);
        }
    }
    return true;
}
