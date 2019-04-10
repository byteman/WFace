#ifndef MODBUS_CHANNEL_H
#define MODBUS_CHANNEL_H
#include <QtGlobal>
#include <QString>
#include "modbus.h"
//modbus的通道抽象接口
class ModbusChannel
{
public:
    ModbusChannel();
    bool set_response_timeout(int ms);
    void setDeviceAddr(int _addr);
    int  getDeviceAddr();
    bool setByteTimeout(int us);
    bool setDelay(int ms);
    bool hasConnectd();
    int  write_register(int reg_addr, int value);
    int  write_registers(int reg_addr, int nb,quint16* value);
    void  sleep();
    int  read_registers(int reg_addr, int nb,quint16* value);
    int  read_input_registers(int reg_addr, int nb,quint16* value);
    bool close();
    void flush();
    void get_rx_tx(int &rx, int &tx);
    virtual int  getType()=0;
protected:
    modbus_t * m_modbus;
    int m_slave_addr;
    int m_tx_cout;
    int m_rx_cout;
    int m_delay_ms;
    bool m_conneted;

};

#endif // ICHANNEL_H
