#ifndef NETCLIENT_H
#define NETCLIENT_H
#include <QTcpSocket>
#include <QByteArray>
#include "netmsg.h"
class NetClient:public QObject
{
    Q_OBJECT
public:
    NetClient(QTcpSocket* socket);
signals:
    void signalMsg(Msg_Head head,void* msg);
    void signalDataReady(QByteArray data);
private slots:

    void onDataReceived();

private:
    QTcpSocket _socket;
    QByteArray _data;
    QByteArray _data_notify;
    void processOneWeight(QByteArray &data);
    void parse();
};

#endif // NETCLIENT_H
