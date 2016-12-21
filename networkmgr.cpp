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
    if(!_tcpServer.listen(QHostAddress::Any,8083))
    {
        return false;
    }
    connect(&_tcpServer,&QTcpServer::newConnection,this,&NetWorkMgr::onNewConection);
}

void NetWorkMgr::addClient(QTcpSocket *_socket)
{
    NetClient* client = new NetClient(_socket);
    _clientList[_socket->peerAddress().toString()] = client;
    connect(client,SIGNAL(onOneWeight(PointWet)),this, SLOT(onOneWeight(PointWet)));
    connect(client,SIGNAL(signalOneWeight(QByteArray)),this, SLOT(onOneWeight(PointWet)));
    QString log = QString("ip %1 port %2 connected").arg(_socket->peerAddress().toString()).arg(_socket->peerPort());
    qDebug() << log;
    //ui->txtLog->append(log);
}

void NetWorkMgr::onNewConection()
{

    QTcpSocket* socket = _tcpServer.nextPendingConnection();
    addClient(socket);
}

void NetWorkMgr::onOneWeight(PointWet wet)
{
    emit SignalOneWeight((QTcpSocket*)sender(),wet);
}

void NetWorkMgr::onDataReady(QByteArray data)
{
    emit SignalDataReady((QTcpSocket*)sender(),data);
}
