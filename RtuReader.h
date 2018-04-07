#ifndef RTUREADER_H
#define RTUREADER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "dcschannel.h"


class RtuReader : public QObject
{
    Q_OBJECT
public:
    explicit RtuReader(QObject *parent = 0);
    bool start(int interval=500);
    bool hasConnected(){return m_connect;}
    bool setDeviceAddr(int addr);
    bool set_response_timeout(int us);

    int  write_register(int reg_addr, int value);
    int  write_registers(int reg_addr, int nb,quint16* value);

    int  read_registers(int reg_addr, int nb,quint16* value);
    int  read_input_registers(int reg_addr, int nb,quint16* value);



    int send_then_recv(quint8 cmd, QByteArray &send_data, QByteArray &recv_data, int want);

    QString port() const;
    void get_rx_tx(int& rx, int &tx);

    bool open(QString port, int baud, char parity, char databit, char stopbit);

    void flush();
public slots:
    void doWork();
private:
    QThread     m_thread;
    DCS_Channel m_rtu;
    int         m_interval;
    bool        m_stop;
    bool        m_connect;
    QMutex      m_mutex;

    int m_slaveAddr;
};

#endif // RTUREADER_H
