#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QtGlobal>
#include <QString>
#include "modbus.h"

class RTU_Modbus
{
public:
    RTU_Modbus();


    bool open(const char* port, int baud, char parity, char databit, char stopbit);
    bool close();
    bool setByteTimeout(int ms);
    bool set_response_timeout(int ms);
    void setDeviceAddr(int _addr);
    int  write_register(int reg_addr, int value);
    int  write_registers(int reg_addr, int nb,quint16* value);
    void flush();
    int  read_registers(int reg_addr, int nb,quint16* value);
    int  read_input_registers(int reg_addr, int nb,quint16* value);
    QString port() const;
    void get_rx_tx(int& rx, int &tx);

private:
    modbus_t * m_modbus;
    int m_slave_addr;
    QString m_port;
    int m_tx_cout;
    int m_rx_cout;
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
