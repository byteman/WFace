#include "MultiModbusReader.h"



MultiModbusReader::MultiModbusReader(QObject *parent):
    ModbusReader(parent)
{

}

void MultiModbusReader::clearChannel()
{
    channels.clear();
}

void MultiModbusReader::pushChannel(ModbusChannel *channel,int addr)
{
    channel->setDeviceAddr(addr);
    channels.push_back(channel);
}


bool MultiModbusReader::hasConnected()
{
    for(int i = 0; i < channels.size(); i++)
    {
        if(!channels[i]->hasConnectd()){
            return false;
        }
    }
    return true;
}

bool MultiModbusReader::setDeviceAddr(int addr)
{
    if(addr >= channels.size() || addr <= 0 ) return false;

    m_current_index = addr;
    return true;

}

int MultiModbusReader::getCurrentDeviceAddr()
{
    return m_current_index;
}

bool MultiModbusReader::setDelay(int ms)
{
    for(int i = 0; i < channels.size(); i++)
    {
        if(!channels[i]->setDelay(ms)){
            return false;
        }
    }
    return true;
}

bool MultiModbusReader::set_response_timeout(int us)
{
    for(int i = 0; i < channels.size(); i++)
    {
        if(!channels[i]->set_response_timeout(us)){
            return false;
        }
    }
    return true;
}

int MultiModbusReader::write_register(int reg_addr, int value)
{
    if(m_current_index < 1) return 0;
    return channels[m_current_index-1]->write_register(reg_addr,value);
}

int MultiModbusReader::write_registers(int reg_addr, int nb, quint16 *value)
{
    if(m_current_index < 1) return 0;
    return channels[m_current_index-1]->write_registers(reg_addr,nb,value);
}

int MultiModbusReader::read_registers(int reg_addr, int nb, quint16 *value)
{
    if(m_current_index < 1) return 0;
    return channels[m_current_index-1]->read_registers(reg_addr,nb,value);
}


int MultiModbusReader::read_input_registers(int reg_addr, int nb, quint16 *value)
{
    if(m_current_index < 1) return 0;
    return channels[m_current_index-1]->read_input_registers(reg_addr,nb,value);
}

void MultiModbusReader::get_rx_tx(int &rx, int &tx)
{
    for(int i = 0; i < channels.size(); i++)
    {
        int rx1,tx1;
        channels[i]->get_rx_tx(rx1,tx1);
        rx+=rx1;
        tx+=tx1;
    }
}

bool MultiModbusReader::close()
{
    bool result = true;
    for(int i = 0; i < channels.size(); i++)
    {
       result |= channels[i]->close();
    }
    return result;
}

void MultiModbusReader::flush()
{
    for(int i = 0; i < channels.size(); i++)
    {
       channels[i]->flush();
    }
}
