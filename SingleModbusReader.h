#ifndef SINGLEMODBUSREADER_H
#define SINGLEMODBUSREADER_H

#include "ModbusReader.h"
#include "ModbusChannel.h"
class SingleModbusReader:public ModbusReader
{
public:
    explicit SingleModbusReader(QObject *parent = 0);

private:
    ModbusChannel *m_modbus;

    // ModbusReader interface
public:
    virtual int  getChannelNum();
    virtual int  getChannelType();
    virtual void setChannel(ModbusChannel* channel);
    virtual bool hasConnected();
    virtual bool setDeviceAddr(int addr);
    virtual int getCurrentDeviceAddr();
    virtual bool setDelay(int ms);
    virtual bool set_response_timeout(int us);
    virtual int write_register(int reg_addr, int value);
    virtual int write_registers(int reg_addr, int nb, quint16 *value);
    virtual int read_registers(int reg_addr, int nb, quint16 *value);
    virtual int read_input_registers(int reg_addr, int nb, quint16 *value);
    virtual void get_rx_tx(int &rx, int &tx);
    virtual bool isConnectd();
    virtual bool close();
    virtual void flush();
};

#endif // SINGLEMODBUSREADER_H
