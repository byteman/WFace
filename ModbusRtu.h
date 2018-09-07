#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include "ModbusChannel.h"


class ModbusRtu:public ModbusChannel
{
public:
    ModbusRtu();
    ModbusRtu(int index);


    bool open(const char* port, int baud, char parity, char databit, char stopbit);
    bool close();
    bool setByteTimeout(int ms);
    bool setDelay(int ms);
    QString port() const;
    void flush();



    void get_rx_tx(int& rx, int &tx);

    bool open(const char* host,int port);
private:

    int m_slave_addr;
    QString m_port;
    int m_tx_cout;
    int m_rx_cout;
    int m_delay_ms;
    bool m_modbus_tcp;
    void sleep();
    int m_index;
};
#endif // MODBUSRTU_H
