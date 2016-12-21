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
    void onOneWeight(PointWet wet);
    void signalOneWeight(QByteArray data);
private slots:
    void onDisConection();
    void onDataReceived();

private:
    QTcpSocket _socket;
    QByteArray _data;
    void processOneWeight(QByteArray &data);
    void parse();
};

#endif // NETCLIENT_H
