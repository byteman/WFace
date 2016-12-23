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
    bool startUpdate(QString dev_ip, QString file);
    bool readPara(QString dev_ip, int para_addr);
Q_SIGNALS:
    void SignalOneMsg(NetClient* _socket,Msg_Head head, void *data);
    void SignalDataReady(NetClient* _socket,QByteArray data);
    void SignalUpdateEvent(NetClient* _client, int evt, UpdateEvtPara para);
private slots:
    void onUpdateEvent(int evt, UpdateEvtPara para);
    void onDisConection();
    void onNewConection();
    void onNetMsg(Msg_Head head, void *data);
    void onDataReady(QByteArray data);
private:
    NetClient* findClient(QString ip);
    void addClient(QTcpSocket* _socket);
    ClientSocketMap _clientList;
    QTcpServer _tcpServer;

};

#endif // NETWORKMGR_H
