#include "netclient.h"
#include <QDebug>
#include "crc.h"
#include <QHostAddress>
NetClient::NetClient(QTcpSocket* socket):
    _socket(socket)
{
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisConection()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onDataReceived()));
}

void NetClient::onDisConection()
{
     QTcpSocket* _socket = static_cast<QTcpSocket*>(sender());

     QString log = QString("ip %1 port %2 disconnectd!").arg(_socket->peerAddress().toString()).arg(_socket->peerPort());
     //ui->txtLog->append(log);
     qDebug() << log;
     _socket->deleteLater();
}
void NetClient::processOneWeight(QByteArray& data)
{
    PointWet  pwt;

    if(data.size() != sizeof(PointWet))
    {
        qDebug() << "processOneWeight size error";
        return;
    }
    memcpy(&pwt, data.data(), sizeof(PointWet));
    emit onOneWeight(pwt);
}



void NetClient::parse()
{
    static bool waitHead = true;

    static Msg_Head head;
    while(_data.size() >= 0)
    {
        if(waitHead)
        {
            if(_data.size() >= 7)
            {
                memcpy(&head,_data.data(),sizeof(Msg_Head));
                _data.remove(0,sizeof(Msg_Head));
                waitHead = false;
            }
        }
        else
        {
            if(_data.size() < (head.len+2) )
            {
                break;
            }
            //myData.remove(0,head.len+2);
            unsigned short crc =u16CRC_Calc16((const uint8_t*)_data.data(),head.len);
            unsigned short crc_data = (_data[head.len+1]<<8)+_data[head.len];

            if(crc != crc_data)
            {
                _data.remove(0,head.len+2);
                continue;
            }
            _data.remove(head.len,2);
            switch(head.cmd)
            {
                case 0:
                    processOneWeight(_data);
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
            }
            waitHead = true;
            _data.remove(0,head.len);
        }
    }

}
void NetClient::onDataReceived()
{
    QTcpSocket* client = static_cast<QTcpSocket*>(sender());
    QByteArray data = client->readAll();

    _data.append(data);
    parse();
    emit signalOneWeight(data);
    //ui->txtLog->append(data);
    //client->write(data);
    //client->flush();

}
