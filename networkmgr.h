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
    bool readPara(int para_addr,QString dev_id="");
    bool writePara(int para_addr,QByteArray data,QString dev_id="");
    int  getOnLineClients(QStringList &list);
    bool setCurrentClient(QString dev_id);
    bool reset();
Q_SIGNALS:
    void SignalOneMsg(NetClient* _socket,Msg_Head head, void *data);
    void SignalDataReady(NetClient* _socket,QByteArray data);
    void SignalUpdateEvent(NetClient* _client, int evt, UpdateEvtPara para);
    void SignalNewClient(NetClient* _client);
    void SignalRemoveClient(int ipaddr);
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
    NetClient* _curClient;

};

#endif // NETWORKMGR_H
