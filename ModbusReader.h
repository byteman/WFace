#ifndef MODBUSREADER_H
#define MODBUSREADER_H


#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>

class ModbusReader : public QObject
{
    Q_OBJECT
public:
    explicit ModbusReader(QObject *parent = 0);
    bool start(int interval=500);
    virtual bool hasConnected(){return m_connect;}
    virtual bool setDeviceAddr(int addr)=0;
    virtual int  getCurrentDeviceAddr(void);
    virtual int  getVirtualCurrentDeviceAddr(void);
    //virtual int  getRealDeviceAddr(void);
    virtual bool setDelay(int ms)=0;
    virtual bool set_response_timeout(int us)=0;

    virtual int  write_register(int reg_addr, int value)=0;
    virtual int  write_registers(int reg_addr, int nb,quint16* value)=0;

    virtual int  read_registers(int reg_addr, int nb,quint16* value) = 0;
    virtual int  read_input_registers(int reg_addr, int nb,quint16* value)=0;

    virtual void get_rx_tx(int& rx, int &tx) = 0;

    virtual bool close() = 0;
    virtual void flush() = 0;
    virtual int  getChannelType()=0;
    virtual int  getChannelNum()=0;
public slots:
    void doWork();
protected:
    QThread     m_thread;
    int         m_interval;
    bool        m_stop;
    bool        m_connect;
    QMutex      m_mutex;

    int m_slaveAddr;
};

#endif // MODBUSREADER_H
