#include "cornhandler.h"

#include "modbus.h"
#include "adc102.h"
#include <qdebug.h>


CornHandler::CornHandler(RtuReader *rtu):
    CmdHandler(rtu)
{

}
//读取角差系数
bool CornHandler::paraRead(void)
{
    quint16 values[17];

    postReadRegs(REG_4B_CORN_K,4);
    if(4 == _rtu->read_registers(REG_4B_CORN_K,4,values))
    {
        QList<float> chanK;
        for(int i = 0; i < 4; i++)
        {
            float k = (values[0]+(values[1]<<16))/1000;
            chanK.push_back(k);
        }

        emit chanKReadResult(chanK);
        return true;
    }


    return false;
}
bool CornHandler::doWork()
{

    qDebug() << "doWork";
    if(_rtu)
    {
        //qDebug() << "calib work";
        if(!bInit)
        {
            bInit = paraRead();
        }
        quint16 values[8];
        if(4 == _rtu->read_registers(REG_4B_CHANNEL_AD,4,values))
        {
            //定时读取各路通道AD值.
            QList<qint32> chanAD;
            chanAD.push_back(values[0]+(values[1]<<16));
            chanAD.push_back(values[2]+(values[3]<<16));
            chanAD.push_back(values[4]+(values[5]<<16));
            chanAD.push_back(values[6]+(values[7]<<16));

            emit chanADReadResult(chanAD);

        }
        this->msleep(500);
        return false;
    }

    return true;


}

bool CornHandler::init()
{
    return true;
}

bool CornHandler::start()
{
    quint16 value = 0;

    return postWriteRegs(REG_2B_AUTO_CORN, 1, &value);
}

bool CornHandler::calib(int index)
{
    if( (index < 1) || (index > 4)) return false;
    quint16 value = index;

    return postWriteRegs(REG_2B_AUTO_CORN, 1, &value);
}
bool CornHandler::stop()
{
    quint16 value = 5;

    return postWriteRegs(REG_2B_AUTO_CORN, 1, &value);
}
