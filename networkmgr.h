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

Q_SIGNALS:
    void SignalOneWeight(QTcpSocket* _socket,PointWet wet);
    void SignalDataReady(QTcpSocket* _socket,QByteArray data);
private slots:
    void onNewConection();
    void onOneWeight(PointWet wet);
    void onDataReady(QByteArray data);
private:
    void addClient(QTcpSocket* _socket);
    ClientSocketMap _clientList;
    QTcpServer _tcpServer;
};

#endif // NETWORKMGR_H
