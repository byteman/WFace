#ifndef NETCLIENT_H
#define NETCLIENT_H
#include <QTcpSocket>
#include <QByteArray>
#include <QTimer>
#include "netmsg.h"
enum UpdateState{
    UPDATE_IDEL = 0,
    UPDATE_START,
    UPDATE_DATA,
    UPDATE_END
};
enum UpdateEvent{
    UEVT_TIMEOUT, //���ͳ�ʱ
    UEVT_START,
    UEVT_DATA,
    UEVT_END,
    UEVT_OK,
    UEVT_DISCONNECT
};
struct UpdateEvtPara
{
    int cur_pkg_index;
    int total_pkg_index;
};
class NetClient:public QObject
{
    Q_OBJECT
public:
    NetClient(QTcpSocket* socket);
    QTcpSocket* getSocket();
    void processUpdateAck();

    bool isUpdateTimeout();
    void updateComplete();
    void sendUpdateEvent(int evt);
    int  StartUpgrade(QString file,int fileType=1);
    bool readPara(int  para_addr);
    bool writePara(int para_addr, QByteArray para);
    QString getID();
    bool reset();
    void processOnline(Msg_Head &head, QByteArray &data);
signals:
    void signalMsg(Msg_Head head,void* msg);
    void signalDataReady(QByteArray data);
    void signalUpdateEvent(int evt, UpdateEvtPara para);
private slots:

    void onDataReceived();
    void onLineTimerHandler();
    void timerHandler();
private:
    QTcpSocket *_socket;
    QByteArray _updateData;
    QByteArray _data;
    QByteArray _data_notify;
    QString    m_file_name;
    QTimer      m_timer;
    QTimer      m_online_timer;
    int m_file_type;
    int m_packet_index;
    int m_total_packet;
    int m_timeout;
    int m_timeout_retry;
    quint16 m_dev_id;
    QString m_car_id;
    UpdateState _updateState;
    bool m_online;
    int  m_online_timeout;
    void processOneWeight(Msg_Head& head,QByteArray &data);
    void processUpdateAck(Msg_Head& head,QByteArray& data);
    void parse();
    bool send_update_packet(QByteArray data);
    bool sendpacket(quint8 cmd,quint8 oper, QByteArray data);
    void sendUpdateStartRequest();
    void sendUpdateStopRequest();
    void sendUpdateData(int index);

};

#endif // NETCLIENT_H
