#ifndef NETWORKMGR_H
#define NETWORKMGR_H
#include <QTcpServer>
#include <QTcpSocket>
#include "netclient.h"
#include <map>
typedef std::map<QTcpSocket*,NetClient*> ClientSocketMap;
class NetWorkMgr:public QObject
{
    Q_OBJECT
public:
    NetWorkMgr();
    bool start(int port);
    bool stop();

Q_SIGNALS:
    void SignalOneMsg(NetClient* _socket,Msg_Head head, void *data);
    void SignalDataReady(NetClient* _socket,QByteArray data);
private slots:
    void onDisConection();
    void onNewConection();
    void onNetMsg(Msg_Head head, void *data);
    void onDataReady(QByteArray data);
private:
    void addClient(QTcpSocket* _socket);
    ClientSocketMap _clientList;
    QTcpServer _tcpServer;
};

#endif // NETWORKMGR_H
