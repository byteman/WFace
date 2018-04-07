#include "calibhandler.h"

#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>
#include "command.h"

CalibHandler::CalibHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_dot(0),
    m_sensor(0)
{
    for(int i = 0 ;i < 6; i++)
    {
        m_set_calib_points[i] = false;
        m_read_calib_points[i] = false;
    }
}

bool CalibHandler::paraRead(void)
{
    quint16 values[17];

    if(1 == _rtu->read_registers(REG_2B_SENSOR_NUM,1,values))
    {

        m_sensor = (values[0] > 8)?4:values[0];
        if(4 == _rtu->read_registers(REG_FULL_SPAN,4,values))
        {
            quint32 sensor_full_span = values[0]+(values[1]<<16);
            quint32 sensor_mv = values[2]+(values[3]<<16);
            emit calibParaResult(sensor_mv, sensor_full_span);
            if(1 == _rtu->read_registers(REG_DOT,1,values))
            {
                m_dot = values[0];
                return true;
            }
        }
    }


    return false;
}
bool CalibHandler::doWork()
{

    if(_rtu)
    {
        //qDebug() << "calib work";
        if(!bInit)
        {
            //bInit = paraRead();
        }


        this->msleep(100);
        return false;
    }

    return true;


}

bool CalibHandler::init()
{
    return true;
}

bool CalibHandler::calibZero(quint8 addr)
{
   _rtu->setDeviceAddr(addr);
   QByteArray outArr;
   int res = _rtu->send_then_recv(CMD_CUSTOM_CALIB_ZERO,QByteArray(),outArr,0);
   return res > 0;

}
bool CalibHandler::clearZero(quint8 addr)
{
   _rtu->setDeviceAddr(addr);
   QByteArray outArr;
   int res = _rtu->send_then_recv(CMD_CLEAR_ZERO,QByteArray(),outArr,0);
   return res > 0;

}


bool CalibHandler::calibWgt(quint8 addr)
{
    _rtu->setDeviceAddr(addr);
    QByteArray outArr;
    int res = _rtu->send_then_recv(CMD_CUSTOM_CALIB_WGT,QByteArray(),outArr,0);
    return res > 0;
}
