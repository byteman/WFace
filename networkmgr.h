#ifndef NETWORKMGR_H
#define NETWORKMGR_H
#include <QTcpServer>
#include <QTcpSocket>
#include "netclient.h"
#include <map>
typedef std::map<QString,NetClient*> ClientSocketMap;
class NetWorkMgr:public QObject
{
    Q_OBJECT
public:
    NetWorkMgr();
    bool start(int port);
    bool stop();


private slots:
    void onNewConection();
private:
    void addClient(QTcpSocket* _socket);
    ClientSocketMap _clientList;
    QTcpServer _tcpServer;
};

#endif // NETWORKMGR_H
