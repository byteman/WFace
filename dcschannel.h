#ifndef DCS_CHANNEL_H
#define DCS_CHANNEL_H

#include <QtGlobal>
#include <QString>


class DCS_Channel
{
public:
    DCS_Channel();


    bool open(const char* port, int baud, char parity, char databit, char stopbit);
    bool close();
    bool setByteTimeout(int us);
    bool set_response_timeout(int us);
    void setDeviceAddr(int _addr);
    void setDeviceId(int id);
    int  write_register(int reg_addr, int value);
    int  write_registers(int reg_addr, int nb,quint16* value);
    void flush();
    int  read_registers(int reg_addr, int nb,quint16* value);
    int  read_input_registers(int reg_addr, int nb,quint16* value);
    QString port() const;
    void get_rx_tx(int& rx, int &tx);
    int send_then_recv(quint8 cmd, QByteArray &send_data, QByteArray &recv_data, int want);

private:
    int m_port_num;
    int m_slave_addr;
    int m_slave_id;
    int m_byte_timeout_ms;
    QString m_port;
    int m_tx_cout;
    int m_rx_cout;

    int send(quint8 cmd, QByteArray &data);
    int recv(QByteArray &data, quint32 want);

};


#endif // MYMODBUS_H
