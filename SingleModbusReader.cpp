#include "SingleModbusReader.h"

SingleModbusReader::SingleModbusReader(QObject *parent):
    ModbusReader(parent),
    m_modbus(NULL)
{

}


void SingleModbusReader::setChannel(ModbusChannel *channel)
{
    m_modbus = channel;
}

bool SingleModbusReader::setDeviceAddr(int addr)
{
    bool ret = true;
    if(addr == -1){
        return false;
    }
    m_slaveAddr = addr;
    if(m_modbus == NULL) return false;
    m_modbus->setDeviceAddr(addr);
    m_connect = ret;

    return m_connect;
}



bool SingleModbusReader::hasConnected()
{
    return m_connect;
}



int SingleModbusReader::getCurrentDeviceAddr()
{
    if(m_modbus == NULL) return -1;
    return m_modbus->getDeviceAddr();
}

bool SingleModbusReader::setDelay(int ms)
{
    if(m_modbus == NULL) return false;
    return m_modbus->setDelay(ms);
}

bool SingleModbusReader::set_response_timeout(int us)
{
    if(m_modbus == NULL) return false;
    return m_modbus->set_response_timeout(us);
}

int SingleModbusReader::write_register(int reg_addr, int value)
{
    if(m_modbus == NULL) return 0;
    return m_modbus->write_register(reg_addr,value);
}

int SingleModbusReader::write_registers(int reg_addr, int nb, quint16 *value)
{
    if(m_modbus == NULL) return 0;
    return m_modbus->write_registers(reg_addr,nb,value);
}

int SingleModbusReader::read_registers(int reg_addr, int nb, quint16 *value)
{
    if(m_modbus == NULL) return 0;
    return m_modbus->read_registers(reg_addr,nb,value);
}

int SingleModbusReader::read_input_registers(int reg_addr, int nb, quint16 *value)
{
    if(m_modbus == NULL) return 0;
    return m_modbus->read_input_registers(reg_addr,nb,value);
}

void SingleModbusReader::get_rx_tx(int &rx, int &tx)
{
    if(m_modbus == NULL) return;
    return m_modbus->get_rx_tx(rx,tx);
}

bool SingleModbusReader::isConnectd()
{
    if(m_modbus == NULL) return false;
    return m_modbus->hasConnectd();
}

bool SingleModbusReader::close()
{
    if(m_modbus == NULL) return false;
    return m_modbus->close();
}

void SingleModbusReader::flush()
{
    if(m_modbus == NULL) return;
    return m_modbus->flush();
}
