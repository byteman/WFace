#include "networkmgr.h"
#include <QDebug>
#include <QHostAddress>
#include "netclient.h"

NetWorkMgr::NetWorkMgr()
{

}

bool NetWorkMgr::start(int port)
{
    _tcpServer.setMaxPendingConnections(100);
    if(!_tcpServer.listen(QHostAddress::Any,port))
    {
        return false;
    }
    connect(&_tcpServer,&QTcpServer::newConnection,this,&NetWorkMgr::onNewConection);
}

void NetWorkMgr::addClient(QTcpSocket *_socket)
{
    NetClient* client = new NetClient(_socket);
    _clientList[_socket] = client;
    connect(client,SIGNAL(signalMsg(Msg_Head ,void*)),this, SLOT(onNetMsg(Msg_Head,void*)));
    connect(client,SIGNAL(signalDataReady(QByteArray)),this, SLOT(onDataReady(QByteArray)));
    connect(_socket,SIGNAL(disconnected()),this,SLOT(onDisConection()));
    QString log = QString("ip %1 port %2 connected").arg(_socket->peerAddress().toString()).arg(_socket->peerPort());
    qDebug() << log;
    //ui->txtLog->append(log);
}
void NetWorkMgr::onDisConection()
{
     QTcpSocket* _socket = static_cast<QTcpSocket*>(sender());

     QString log = QString("ip %1 port %2 disconnectd!").arg(_socket->peerAddress().toString()).arg(_socket->peerPort());
     //ui->txtLog->append(log);
     qDebug() << log;

     ClientSocketMap::iterator iter=_clientList.find(_socket);
     if(iter!=_clientList.end())
     {
         NetClient* client = _clientList[_socket];
         if(client != NULL) delete client;
         _clientList.erase(iter);
     }
     _socket->deleteLater();
}

void NetWorkMgr::onNewConection()
{

    QTcpSocket* socket = _tcpServer.nextPendingConnection();
    addClient(socket);
}

void NetWorkMgr::onNetMsg(Msg_Head head, void *data)
{
    emit SignalOneMsg((QTcpSocket*)sender(),head,data);
}

void NetWorkMgr::onDataReady(QByteArray data)
{
    emit SignalDataReady((QTcpSocket*)sender(),data);
}
