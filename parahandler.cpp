#include "parahandler.h"
#include "command.h"
#include "Logger.h"
ParaHandler::ParaHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_write(false)
{

}

int ParaHandler::getDot()
{
    return 0;
}

bool ParaHandler::_paraSave(Para &_para,int &reg)
{
    if(_rtu)
    {
        QByteArray outArr;

        int result = _rtu->send_then_recv(CMD_CUSTOM_WRITE_PARAM, _para.toByteArray(), outArr,0);
        if(result <= 0){
            return false;
        }
        reg = 0;
        return true;
    }
    return false;
}
bool ParaHandler::paraSave(Para _para)
{
    m_write = true;
    m_para = _para;
    return m_write;
}
quint16 ParaHandler::getU16(QByteArray &buf,int index)
{
    quint8 hi = buf[index];
    quint8 lo = buf[index+1];

    return (hi<<8) + lo;
}
bool ParaHandler::paraRead(Para &_para)
{

    QByteArray outArr;
    _rtu->flush();
    int result = _rtu->send_then_recv(CMD_CUSTOM_READ_PARAM, QByteArray(), outArr,0);
    if(result <= 0){
        _para.result = 1;
        return false;
    }
    if(outArr.size() < 8) return false;
    _para.sensor_num = getU16(outArr,2);
    _para.read_time_out = getU16(outArr,4);
    _para.limit = getU16(outArr,6);
    _para.result = 0;
    emit paraReadResult(_para);
    return true;
}


bool ParaHandler::doWork()
{
    if(_rtu)
    {
       if(!bInit)
       {
          if(!paraRead(m_para))
          {

          }
          bInit = true;
       }
       if(m_write)
       {
           int reg = 0;
           _paraSave(m_para,reg);
           m_write = false;
           emit paraWriteResult(reg);
       }


    }
    else
    {
        return true;
    }
    return false;
}

bool ParaHandler::queryErrorSensor(SensorErrInfoList &list)
{
    QByteArray outArr;
    _rtu->setDeviceAddr(0);
    int res = _rtu->send_then_recv(CMD_QUERY_ERROR_SENSOR,QByteArray(),outArr,0);
    if(res <= 0){
        return false;
    }
    quint8 sensor_id = outArr[0];
    quint8 sensor_num = outArr[1];

    outArr.remove(0,2);
    int num = outArr.size() / 2;
    if(sensor_num != num)
    {
        LOG_ERROR("sensor num=%d, get num = %d",sensor_num,num);
        return false;
    }
    for(int i = 0; i < num ;i++)
    {
        SensorErrInfo info;
        info.addr = outArr[i*2];
        info.state.StateValue = outArr[i*2 + 1];
        list.push_back(info);
    }

    return true;
}

bool ParaHandler::queryChangeSensor(SensorWgtInfoList &list)
{
    QByteArray outArr;
    _rtu->setDeviceAddr(0);
    int res = _rtu->send_then_recv(CMD_QUERY_CHANGE_SENSOR,QByteArray(),outArr,0);
    if(res <= 0){
        return false;
    }
    quint8 sensor_id = outArr[0];
    quint8 sensor_num = outArr[1];

    outArr.remove(0,2);
    int num = outArr.size() / 4;
    if(sensor_num != num)
    {
        LOG_ERROR("sensor num=%d, get num = %d",sensor_num,num);
        return false;
    }
    for(int i = 0; i < num ;i++)
    {
        SensorWgtInfo info;
        info.addr = outArr[i*2];
        info.state.StateValue = outArr[i*2 + 1];
        quint8 w1 = outArr[i*2+2];
        quint8 w2 = outArr[i*2+3];

        info.wgt =  (w1 << 8) +w2;
        list.push_back(info);
    }

    return true;
}

bool ParaHandler::modifyAddr(quint8 addr)
{
    QByteArray outArr;
    _rtu->setDeviceAddr(0);
    QByteArray inArr;
    inArr.append(addr);
    int res = _rtu->send_then_recv(CMD_CUSTOM_WRITE_SENSOR_ADDR,inArr,outArr,0);
    if(res <= 0){
        return false;
    }
    return true;
}
