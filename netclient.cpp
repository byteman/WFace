#include "netclient.h"
#include "crc.h"
#include <QDebug>
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>

#define MAX_RETRY_COUNT 3
NetClient::NetClient(QTcpSocket* socket):
    _socket(socket),
    m_timeout(2000),
    m_timeout_retry(0),
    m_dev_id(0),
    _updateState(UPDATE_IDEL),
    m_file_type(1)
{

    connect(socket,SIGNAL(readyRead()),this,SLOT(onDataReceived()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timerHandler()));

    m_timer.stop();
}

QTcpSocket* NetClient::getSocket()
{
    return _socket;
}


void NetClient::processOneWeight(Msg_Head& head,QByteArray& data)
{
    PointWet pwt;

    if(data.size() != sizeof(PointWet))
    {
        qDebug() << "processOneWeight size error";
        return;
    }
    memcpy(&pwt, data.data()+sizeof(Msg_Head), sizeof(PointWet));
    qDebug() << pwt.duty;
}

void NetClient::updateComplete()
{
     _updateState = UPDATE_IDEL;
     m_packet_index = 0;
     sendUpdateEvent(UEVT_OK);
}

void NetClient::processUpdateAck(Msg_Head& head,QByteArray& data)
{

    m_timeout_retry = 0;

    qDebug() << "update ack type";

    if(data.size() == 0)
    {
        qDebug() << "processUpdateAck data size=" << data.size();
        return;
    }
    m_timer.stop();
    int err = data[0];
    if(err != 0)
    {
        qDebug() << "update ack err=" << err;
        return;
    }
    switch(_updateState)
    {
        case UPDATE_START:
            sendUpdateData(0);
            break;
        case UPDATE_DATA:
            sendUpdateData(++m_packet_index);
            break;
        case UPDATE_END:
            updateComplete();
            break;
        default:
            break;
    }
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
#if 1
            //build complete packet for crc check;
            QByteArray arr;
            arr.setRawData((const char*)&head,sizeof(Msg_Head));


            _data.push_front(arr);
#endif

#if 1
            //crc16校验.
            unsigned short crc =Reentrent_CRC16((uint8_t*)_data.data(),head.len+sizeof(Msg_Head));
            int crc_index = head.len+sizeof(Msg_Head);
            unsigned short crc_h = (_data[crc_index+1]<<8)&0xFF00;
            unsigned short crc_l = (_data[crc_index])&0x00FF;
            unsigned short crc_data = (crc_h + crc_l);
#endif
            waitHead = true; //这句话一定要要.
            if(crc != crc_data)
            {
                qDebug() << "crc error";
                _data.remove(0,head.len+2+sizeof(Msg_Head));
                continue;
            }
            m_dev_id = head.dev_id;
#if 1
            //构造一个完整的包用于通知.
            _data_notify.clear();
            _data_notify.setRawData(_data.data()+sizeof(Msg_Head), head.len);
            emit signalMsg(head,_data_notify.data());
#endif
            _data.remove(0,sizeof(Msg_Head)); //remove head bytes
            _data.remove(head.len,2); //remove 2 bytes crc

            switch(head.cmd)
            {
                case CMD_DEV2HOST_ONE_WEIGHT:
                    processOneWeight(head,_data);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case CMD_UPDATE:
                    processUpdateAck(head,_data);
                    break;
            }

            _data.remove(0,head.len);
        }
    }

}
bool NetClient::send_update_packet(QByteArray data)
{
    return sendpacket(CMD_UPDATE, OPER_HOST_WRITE_DEV, data);
}


bool NetClient::sendpacket(quint8 cmd, quint8 oper, QByteArray data)
{
    Msg_Head head;
    head.dev_id = m_dev_id;
    head.cmd = cmd; //update;
    head.dir = DIR_HOST_TO_DEV;
    head.len = (data.size()&0xFFFF);
    head.oper = oper;

    QByteArray _send((const char*)&head, sizeof(Msg_Head));

    _send.append(data);
    m_timeout_retry++;
    if(_socket!=NULL)
    {
        qint64 n = _socket->write(_send);
        m_timer.start(m_timeout);
        return (n==_send.size());
    }
    return false;
}
bool NetClient::isUpdateTimeout()
{
    if(m_timeout_retry > MAX_RETRY_COUNT)
    {
        //接收超时.
        _updateState = UPDATE_IDEL;
        m_timer.stop();
        sendUpdateEvent(UEVT_TIMEOUT);
        return true;
    }
    return false;
}
void NetClient::sendUpdateStartRequest()
{
    if(isUpdateTimeout())
    {
        qDebug() << "sendUpdateStartRequest timeout";
        return;
    }
    QByteArray data;
    _updateState = UPDATE_START;

    int size = _updateData.size();
    fileStartDef fileHead;
    memset(&fileHead,0,sizeof(fileStartDef));

    //const char* ptr =  (const char*) m_file_name.toLocal8Bit();
    strcpy(fileHead.file_name,"release.bin"); //文件名
    fileHead.file_size = size; //文件大小
    fileHead.file_type = m_file_type;//升级文件类型
    fileHead.mode = -1;
    data.append((const char*)&fileHead,sizeof(fileStartDef));

    m_packet_index = 0;
    send_update_packet(data);
    sendUpdateEvent(UEVT_START);
    //QTimer::singleShot(m_timeout,this,SLOT(timerHandler()));



}

void NetClient::sendUpdateStopRequest()
{
    qDebug() << "sendUpdateStopRequest";
    if(isUpdateTimeout())
    {
        qDebug() << "sendUpdateStartRequest timeout";
        return;
    }
    QByteArray data;
    _updateState = UPDATE_END;

    int oper = -2;
    data.append((const char*)&oper,sizeof(int));
    sendUpdateEvent(UEVT_END);
    send_update_packet(data);
}
void NetClient::sendUpdateEvent(int evt)
{
    UpdateEvtPara par;
    par.cur_pkg_index = m_packet_index;
    par.total_pkg_index = m_total_packet;
    emit signalUpdateEvent(evt,par);
}
#define UPDATE_PACKET_SIZE 128
void NetClient::sendUpdateData(int index)
{
    int pos = index*UPDATE_PACKET_SIZE;
    qDebug() << "sendUpdateData index=" << index;
    if(isUpdateTimeout())
    {
        qDebug() << "sendUpdateData timeout packet" << m_packet_index;
        return ;
    }
    if(pos >= _updateData.size())
    {
        qDebug() << "sendUpdateData complete";
        //已经读到最后长度了.
        sendUpdateStopRequest();
        return;
    }
    m_packet_index  = index;

    QByteArray data;


    int size = (_updateData.size() > UPDATE_PACKET_SIZE)?UPDATE_PACKET_SIZE:_updateData.size();
    data.append((const char*)&index,sizeof(int)); //包序号
    data.append((const char*)&size,sizeof(int)); //包长度

    qDebug() << "send" << index << " data " << size;
    data.append(_updateData.mid(pos,size));
//    for(;size < UPDATE_PACKET_SIZE;size++)
//    {
//        data.append(0xFF);
//    }
    qDebug() << "packet size" << data.size();
    send_update_packet(data);

    sendUpdateEvent(UEVT_DATA);
    _updateState = UPDATE_DATA;
    //QTimer::singleShot(m_timeout,this,SLOT(timerHandler()));
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

int NetClient::StartUpgrade(QString file,int fileType)
{

    QFile ufile(file);
    if(ufile.open(QFile::ReadOnly) == false)
    {
        qDebug() << "can not not " << file;
        return -1;
    }
    m_file_type = fileType;
    _updateData = ufile.readAll();
    QFileInfo info = QFileInfo(file);

    m_file_name = info.fileName();
    sendUpdateStartRequest();

}

bool NetClient::readPara(int para_addr)
{
    QByteArray data;
    return sendpacket(para_addr, OPER_HOST_READ_DEV,data);
}

bool NetClient::writePara(int para_addr, QByteArray para)
{
    return sendpacket(para_addr, OPER_HOST_WRITE_DEV,para);
}

QString NetClient::getID()
{
    return _socket->peerAddress().toString();
}

bool NetClient::reset()
{
    QByteArray data;
    data.clear();
    return sendpacket(CMD_RESET,OPER_HOST_WRITE_DEV,data);
}

void NetClient::timerHandler()
{

    switch(_updateState)
    {
        case UPDATE_START:
            sendUpdateStartRequest();
            break;
        case UPDATE_DATA:
            sendUpdateData(m_packet_index);
        case UPDATE_END:
            sendUpdateStopRequest();
            break;
        default:
            qDebug() << "timeout idle";
            break;

    }
}
