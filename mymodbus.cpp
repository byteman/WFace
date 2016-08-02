#include "mymodbus.h"
#include "modbus-rtu.h"
RTU_Modbus::RTU_Modbus():
    m_modbus(NULL),
    m_slave_addr(0)
{

}

bool RTU_Modbus::open(const char* port, int baud, char parity, char databit, char stopbit)
{
    if(m_modbus)
    {
        modbus_close( m_modbus );
        modbus_free( m_modbus );
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
    return true;
}

bool RTU_Modbus::close()
{
    modbus_close(m_modbus);
    return true;
}

void RTU_Modbus::setDeviceAddr(int _addr)
{
    m_slave_addr = _addr;
    modbus_set_slave(m_modbus,_addr);
}
int  RTU_Modbus::write_registers(int reg_addr, int nb,quint16* value)
{
    int ret = modbus_write_registers( m_modbus, reg_addr,nb,value );
    return ret;
}


int  RTU_Modbus::write_register(int reg_addr, int value)
{
    int ret = modbus_write_register( m_modbus, reg_addr,value );
    return ret;
}
int  RTU_Modbus::read_registers(int reg_addr, int nb,quint16* value)
{

    int ret = modbus_read_registers( m_modbus, reg_addr,nb,value );
    return ret;
}
//cmd = 7
int  RTU_Modbus::read_input_registers(int reg_addr, int nb,quint16* value)
{

    int ret = modbus_read_input_registers( m_modbus, reg_addr,nb,value );
    return ret;
}
