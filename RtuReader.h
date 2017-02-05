#ifndef RTUREADER_H
#define RTUREADER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "mymodbus.h"

typedef enum{
    REG_SLAVE_ADDR=31,
}RegTA51;
typedef enum {
    REG_ERROR_OK=0,
    REG_ERROR_TIMEOUT
}RegError;
struct RegOperCmd{
    RegOperCmd()
    {
        isRead = false;
        error  = REG_ERROR_OK;
    }
    bool    isRead;
    int     reg_addr;
    int     reg_num;
    int     error; //错误码 0成功 1超时.
    quint16 reg_value[32];
};

class RtuReader : public QObject
{
    Q_OBJECT
public:
    explicit RtuReader(QObject *parent = 0);
    bool start(int interval=500);
    bool setAddr(int addr);
    bool set_response_timeout(int us);
    bool open(QString port, int baud, char parity, char databit, char stopbit);
    bool addCmd(RegOperCmd cmd);
signals:
    void OperationResult(RegOperCmd value);
public slots:
    void doWork();
private:
    QThread     m_thread;
    RTU_Modbus  m_rtu;
    int         m_interval;
    bool        m_stop;
    bool        m_connect;
    QMutex      m_mutex;
    QList<RegOperCmd> m_cmdlist;
    int m_slaveAddr;
};

#endif // RTUREADER_H
