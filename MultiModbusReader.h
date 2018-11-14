#ifndef MULTIMODBUSREADER_H
#define MULTIMODBUSREADER_H
#include <QVector>
#include "ModbusReader.h"
#include "ModbusChannel.h"
class MultiModbusReader:public ModbusReader
{
public:
    explicit MultiModbusReader(QObject *parent = 0);

    // ModbusReader interface
public:
    void clearChannel();
    void pushChannel(ModbusChannel* channel,int addr);

    virtual bool hasConnected();
    virtual bool setDeviceAddr(int addr);
    virtual int  getCurrentDeviceAddr();
    virtual bool setDelay(int ms);
    virtual bool set_response_timeout(int us);
    virtual int  write_register(int reg_addr, int value);
    virtual int  write_registers(int reg_addr, int nb, quint16 *value);
    virtual int  read_registers(int reg_addr, int nb, quint16 *value);
    virtual int  read_input_registers(int reg_addr, int nb, quint16 *value);
    virtual void get_rx_tx(int &rx, int &tx);

    virtual bool close();
    virtual void flush();
private:
    QVector<ModbusChannel*> channels;
    int m_current_index;
    ModbusChannel* m_current_channel;
    int m_real_addr;

    // ModbusReader interface
public:
    virtual int getRealDeviceAddr();
};

#endif // MULTIMODBUSREADER_H
