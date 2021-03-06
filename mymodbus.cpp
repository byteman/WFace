#include "mymodbus.h"
#include "modbus-rtu.h"
RTU_Modbus::RTU_Modbus():
    m_modbus(NULL),
    m_slave_addr(0),
    m_tx_cout(0),
    m_rx_cout(0)
{

}

bool RTU_Modbus::open(const char* port, int baud, char parity, char databit, char stopbit)
{
    if(m_modbus)
    {
        modbus_close( m_modbus );
        modbus_free( m_modbus );
        m_modbus = NULL;
    }
    m_modbus = modbus_new_rtu(port,
            baud,
            parity,
            databit,
            stopbit );
    if(m_modbus == NULL) return false;
    if( modbus_connect( m_modbus ) == -1 )
    {
        return false;
    }
    m_port = port;
    return true;
}

bool RTU_Modbus::close()
{
    modbus_close(m_modbus);
    return true;
}

bool RTU_Modbus::setByteTimeout(int us)
{
    return (modbus_set_byte_timeout(m_modbus,us/1000000,us%1000000)==0)?true:false;
}

bool RTU_Modbus::set_response_timeout(int us)
{
    return (modbus_set_response_timeout(m_modbus,us/1000000,us%1000000)==0)?true:false;
}

void RTU_Modbus::setDeviceAddr(int _addr)
{
    m_slave_addr = _addr;
    modbus_set_slave(m_modbus,_addr);
}
int  RTU_Modbus::write_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    int ret = modbus_write_registers( m_modbus, reg_addr,nb,value );
    if(nb == ret){
        m_rx_cout+=nb;
    }
    return ret;
}


int  RTU_Modbus::write_register(int reg_addr, int value)
{
    m_tx_cout++;
    int ret = modbus_write_register( m_modbus, reg_addr,value );
    if(1 == ret){
        m_rx_cout++;
    }
    return ret;
}
int  RTU_Modbus::read_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    int ret = modbus_read_registers( m_modbus, reg_addr,nb,value );
    if(nb == ret){
        m_rx_cout+=nb;
    }
    return ret;
}
//cmd = 7
int  RTU_Modbus::read_input_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    int ret = modbus_read_input_registers( m_modbus, reg_addr,nb,value );
    if(nb == ret){
        m_rx_cout+=nb;
    }
    return ret;
}
QString RTU_Modbus::port() const
{
    return m_port;
}

void RTU_Modbus::get_rx_tx(int &rx, int &tx)
{
    rx = m_rx_cout;
    tx = m_tx_cout;
}

