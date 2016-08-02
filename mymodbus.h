#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QtGlobal>
#include "modbus.h"

class RTU_Modbus
{
public:
    RTU_Modbus();


    bool open(const char* port, int baud, char parity, char databit, char stopbit);
    bool close();

    void setDeviceAddr(int _addr);
    int  write_register(int reg_addr, int value);
    int  write_registers(int reg_addr, int nb,quint16* value);

    int  read_registers(int reg_addr, int nb,quint16* value);
    int  read_input_registers(int reg_addr, int nb,quint16* value);
private:
    modbus_t * m_modbus;
    int m_slave_addr;
};

class Modbus_Factory
{
public:
    static Modbus_Factory& get();
    RTU_Modbus* create_RTU_Modbus()
    {
        return new RTU_Modbus();
    }
};
#endif // MYMODBUS_H
