#include "parahandler.h"

ParaHandler::ParaHandler(RtuReader *rtu):
    CmdHandler(rtu),
    m_write(false)
{

}

int ParaHandler::getDot()
{
    return m_para.dot;
}

bool ParaHandler::_paraSave(Para &_para,int &reg)
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
        reg = 3;
        //msleep(300);
       if(1 == _rtu->write_registers(3,1,values))
       {
           reg = 8;
           //msleep(300);
           if(_rtu->write_registers(8,2,values+1) != 2)return false;
           //msleep(300);
           reg = 10;
           if(_rtu->write_registers(10,2,values+3) != 2)return false;
           //msleep(300);
           reg = 12;
           if(_rtu->write_registers(12,2,values+5) != 2)return false;
           //msleep(300);
           reg = 14;
           if(_rtu->write_registers(14,6,values+7) != 6)return false;
                //msleep(300);
                reg = 96;
               _rtu->write_registers(96,1,&_para.adRate);
               //msleep(300);
               _rtu->read_registers(96,1,values);
               //msleep(300);
               if(_para.adRate == values[0])
               {
                   reg = 0;
                   return true;
               }


       }

    }
    return false;
}
bool ParaHandler::paraSave(Para _para)
{
    m_write = true;
    m_para = _para;
    return m_write;
}

bool ParaHandler::paraRead(Para &_para)
{
    quint16 values[17];
    if(17 == _rtu->read_registers(3,17,values))
    {

        //m_para.dot = values[0];
        _para.dot = values[0];
        _para.div_high = values[5];
        _para.div_low = values[6];
        _para.span_high = values[7]+(values[8]<<16);
        _para.span_low = values[9]+(values[10]<<16);
        _para.unit = values[11];
        _para.pwr_zero_span = values[12];
        _para.hand_zero_span = values[13];
        _para.zero_track_span = values[14];
        _para.stable_span = values[15];
        _para.filter_level = values[16];
        //msleep(300);
        //memcpy(&m_para,&values[0],sizeof(values));
        if(10 == _rtu->read_registers(26,10,values))
        {
            _para.sensor_full_span = values[0]+(values[1]<<16);
            _para.sensor_mv = values[2]+(values[3]<<16);
            _para.slave_addr = values[4];
            _para.version = values[5];
            _para.serial = values[8]+(values[9]<<16);
            //msleep(300);
            if(1 == _rtu->read_registers(96,1,&_para.adRate))
            {
                emit paraReadResult(_para);
                return true;
            }

        }

    }
    return false;
}

bool ParaHandler::doWork()
{
    if(_rtu)
    {
       if(!bInit)
       {
          bInit =  paraRead(m_para);
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
