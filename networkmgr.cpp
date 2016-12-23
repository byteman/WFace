#include "networkmgr.h"
#include <QDebug>
#include <QHostAddress>
#include "netclient.h"

NetWorkMgr::NetWorkMgr():
    _curClient(NULL)
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

bool NetWorkMgr::startUpdate(QString dev_ip, QString file)
{
    NetClient* client = findClient(dev_ip);
    if(client == NULL)
    {
        return false;
    }
    int err = client->StartUpgrade(file);

    return err==0;

}

bool NetWorkMgr::readPara(QString dev_ip, int para_addr)
{
    NetClient* client = findClient(dev_ip);
    if(client == NULL)
    {
        return false;
    }
    client->readPara(para_addr);
}

int NetWorkMgr::getOnLineClients(QStringList &list)
{
    ClientSocketMap::iterator it = _clientList.begin();

    for(;it != _clientList.end(); it++)
    {
        list.push_back(it->second->getID());
    }
    return list.size();
}

bool NetWorkMgr::setCurrentClient(QString dev_id)
{
    _curClient = findClient(dev_id);
    return true;
}

bool NetWorkMgr::reset()
{
    if(_curClient==NULL) return false;
    return _curClient->reset();
}

void NetWorkMgr::onUpdateEvent(int evt, UpdateEvtPara para)
{
    emit SignalUpdateEvent((NetClient*)sender(), evt, para);
}

void NetWorkMgr::addClient(QTcpSocket *_socket)
{
    NetClient* client = new NetClient(_socket);
    _clientList[_socket] = client;
    connect(client,SIGNAL(signalMsg(Msg_Head ,void*)),this, SLOT(onNetMsg(Msg_Head,void*)));
    connect(client,SIGNAL(signalDataReady(QByteArray)),this, SLOT(onDataReady(QByteArray)));
    connect(client,SIGNAL(signalUpdateEvent(int,UpdateEvtPara)) ,this, SLOT(onUpdateEvent(int,UpdateEvtPara)));
    connect(_socket,SIGNAL(disconnected()),this,SLOT(onDisConection()));

    QString log = QString("ip %1 port %2 connected").arg(_socket->peerAddress().toIPv4Address()).arg(_socket->peerPort());
    qDebug() << log;
    emit SignalNewClient(client);
    //ui->txtLog->append(log);
}
void NetWorkMgr::onDisConection()
{
     QTcpSocket* _socket = static_cast<QTcpSocket*>(sender());

     QString log = QString("ip %1 port %2 disconnectd!").arg(_socket->peerAddress().toIPv4Address()).arg(_socket->peerPort());
     //ui->txtLog->append(log);
     qDebug() << log;
     int ip = _socket->peerAddress().toIPv4Address();
     ClientSocketMap::iterator iter=_clientList.find(_socket);
     if(iter!=_clientList.end())
     {
         NetClient* client = _clientList[_socket];
         if(client != NULL) delete client;
         _clientList.erase(iter);
     }
     _socket->deleteLater();
     emit SignalRemoveClient(ip);
}

void NetWorkMgr::onNewConection()
{

    QTcpSocket* socket = _tcpServer.nextPendingConnection();
    addClient(socket);
}

void NetWorkMgr::onNetMsg(Msg_Head head, void *data)
{
    emit SignalOneMsg((NetClient*)sender(),head,data);
}

void NetWorkMgr::onDataReady(QByteArray data)
{
    emit SignalDataReady((NetClient*)sender(),data);
}

NetClient *NetWorkMgr::findClient(QString id)
{
    ClientSocketMap::iterator it = _clientList.begin();
    NetClient* client = NULL;
    for(;it != _clientList.end(); it++)
    {
        QString _id = it->second->getID();
        if(_id == id)
        {
            client = it->second;
            break;
        }
    }
    return client;
}
