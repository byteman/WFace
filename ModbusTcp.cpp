#include "ModbusTcp.h"
#include <QThread>
ModbusTcp::ModbusTcp():
    ModbusChannel()
{

}
void  ModbusTcp::sleep()
{
    QThread::msleep(m_delay_ms);
}
bool ModbusTcp::open(const char* host,int port)
{
    if(m_modbus)
    {
        modbus_close( m_modbus );
        modbus_free( m_modbus );
        m_modbus = NULL;
    }
    m_modbus = modbus_new_tcp(host,
            port);
    if(m_modbus == NULL) return false;
    if( modbus_connect(m_modbus) == -1 )
    {
        modbus_free(m_modbus);
        m_modbus = NULL;
        return false;
    }
    m_port = port;
    return true;
}
