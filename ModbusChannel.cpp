#include "ModbusChannel.h"

ModbusChannel::ModbusChannel() :
    m_modbus(NULL),
    m_slave_addr(0),
    m_tx_cout(0),
    m_rx_cout(0)
{

}

bool ModbusChannel::close()
{
    modbus_close(m_modbus);
    return true;
}
void  ModbusChannel::sleep()
{

}
bool ModbusChannel::setByteTimeout(int us)
{
    return (modbus_set_byte_timeout(m_modbus,us/1000000,us%1000000)==0)?true:false;
}

bool ModbusChannel::setDelay(int ms)
{
    if( (ms < 0) || (ms > 2000)) return false;
    m_delay_ms = ms;
    return true;
}

bool ModbusChannel::hasConnectd()
{
    return m_conneted;
}

bool ModbusChannel::set_response_timeout(int us)
{
    return (modbus_set_response_timeout(m_modbus,us/1000000,us%1000000)==0)?true:false;
}

void ModbusChannel::setDeviceAddr(int _addr)
{
    m_slave_addr = _addr;
    modbus_set_slave(m_modbus,_addr);
}

int ModbusChannel::getDeviceAddr()
{
    return m_slave_addr;
}

int  ModbusChannel::write_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    int ret = modbus_write_registers( m_modbus, reg_addr,nb,value );
    if(nb == ret){
        m_rx_cout+=nb;
    }
    sleep();
    return ret;
}

void ModbusChannel::flush()
{
    modbus_flush(m_modbus);
}


int  ModbusChannel::write_register(int reg_addr, int value)
{
    m_tx_cout++;
    int ret = modbus_write_register( m_modbus, reg_addr,value );
    if(1 == ret){
        m_rx_cout++;
    }
    sleep();
    return ret;
}
int  ModbusChannel::read_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    int ret = modbus_read_registers( m_modbus, reg_addr,nb,value );
    if(nb == ret){
        m_rx_cout+=nb;
    }
    sleep();
    return ret;
}
//cmd = 7
int  ModbusChannel::read_input_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    int ret = modbus_read_input_registers( m_modbus, reg_addr,nb,value );
    if(nb == ret){
        m_rx_cout+=nb;
    }
    sleep();
    return ret;
}

void ModbusChannel::get_rx_tx(int &rx, int &tx)
{
    rx = m_rx_cout;
    tx = m_tx_cout;
}

