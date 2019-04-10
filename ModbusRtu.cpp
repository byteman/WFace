#include "ModbusRtu.h"

ModbusRtu::ModbusRtu():
    m_index(0)
{

}

ModbusRtu::ModbusRtu(int index):
    ModbusChannel(),
    m_index(index)
{

}

bool ModbusRtu::open(const char *port, int baud, char parity, char databit, char stopbit)
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

        modbus_free(m_modbus);
        m_modbus = NULL;
        return false;
    }
    modbus_set_error_recovery(m_modbus,MODBUS_ERROR_RECOVERY_PROTOCOL);
    m_port = port;
    m_modbus_tcp = false;
    m_conneted = true;
    return true;

}

int ModbusRtu::getType()
{
    return 0;
}

