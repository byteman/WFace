#ifndef NETWORKMGR_H
#define NETWORKMGR_H
#include <QTcpServer>
#include <QTcpSocket>
#include "netclient.h"
#include <map>
typedef std::map<std::string,NetClient*> ClientSocketMap;
class NetWorkMgr
{
public:
    NetWorkMgr();
    bool start(int port);
    bool stop();
    void addClient(QTcpSocket* _socket);
private:
    ClientSocketMap _clientList;
};

#endif // NETWORKMGR_H
