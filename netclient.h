#ifndef NETCLIENT_H
#define NETCLIENT_H
#include <QTcpSocket>

class NetClient
{
public:
    NetClient(QTcpSocket* socket);
private:
    QTcpSocket _socket;
};

#endif // NETCLIENT_H
