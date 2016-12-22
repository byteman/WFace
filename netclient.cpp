#include "netclient.h"
#include <QDebug>
#include "crc.h"
#include <QHostAddress>
NetClient::NetClient(QTcpSocket* socket):
    _socket(socket)
{

    connect(socket,SIGNAL(readyRead()),this,SLOT(onDataReceived()));
}

QTcpSocket* NetClient::getSocket()
{
    return _socket;
}


void NetClient::processOneWeight(QByteArray& data)
{
    PointWet pwt;

    if(data.size() != sizeof(PointWet)+sizeof(Msg_Head) + 2)
    {
        qDebug() << "processOneWeight size error";
        return;
    }
    memcpy(&pwt, data.data()+sizeof(Msg_Head), sizeof(PointWet));
    qDebug() << pwt.duty;
}



void NetClient::parse()
{
    static bool waitHead = true;

    static Msg_Head head;
    while(_data.size() >= 0)
    {
        if(waitHead)
        {
            if(_data.size() >= sizeof(Msg_Head))
            {
                memcpy(&head,_data.data(),sizeof(Msg_Head));
                _data.remove(0,sizeof(Msg_Head));
                waitHead = false;
            }
            else break;
        }
        else
        {
            if(_data.size() < (head.len+2) )
            {
                break;
            }
            //myData.remove(0,head.len+2);
            QByteArray arr;
            arr.setRawData((const char*)&head,sizeof(Msg_Head));


            _data.push_front(arr);


            unsigned short crc =Reentrent_CRC16((uint8_t*)_data.data(),head.len+sizeof(Msg_Head));
            int crc_index = head.len+sizeof(Msg_Head);
            unsigned short crc_h = (_data[crc_index+1]<<8)&0xFF00;
            unsigned short crc_l = (_data[crc_index])&0x00FF;
            unsigned short crc_data = (crc_h + crc_l);
            waitHead = true;
            if(crc != crc_data)
            {
                qDebug() << "crc error";
                _data.remove(0,head.len+2+sizeof(Msg_Head));
                continue;
            }
            _data_notify.clear();
            _data_notify.setRawData(_data.data()+sizeof(Msg_Head), head.len);
            emit signalMsg(head,_data_notify.data());

            switch(head.cmd)
            {
                case 1:
                    processOneWeight(_data);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }
            waitHead = true;
            _data.remove(0,head.len+2+sizeof(Msg_Head));
        }
    }

}
void NetClient::onDataReceived()
{
    QTcpSocket* client = static_cast<QTcpSocket*>(sender());
    QByteArray data = client->readAll();
    emit signalDataReady(data);
    qDebug() << data;
    _data.append(data);
    parse();

    //ui->txtLog->append(data);
    //client->write(data);
    //client->flush();

}
