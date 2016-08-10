#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H
#include <QThread>
#include "QString"
#include "cmdhandler.h"
#include <QException>
#include "mymodbus.h"
class MyException : public QException
{
public:
    MyException(QString error);
    void raise() const { throw *this; }
    MyException *clone() const { return new MyException(*this); }
    QString getError() const;
    void setError(const QString &value);

private:
    QString error;
};

//class UpdateHandler : public CmdHandler,public QThread
 class UpdateHandler : public QThread
{
     Q_OBJECT
public:
    UpdateHandler(RTU_Modbus*  rtu);
    bool dorun();
    bool startUpdate(QString file);
    bool stop();
    void sendMessage(int result, int pos, int total);
signals:
    void updateResult(int result, int pos, int total);
private:
    void run();
    int m_port;
    int m_state;
    RTU_Modbus *_rtu;
    QString m_file;
};

#endif // UPDATEHANDLER_H
