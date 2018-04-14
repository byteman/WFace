#include "dcschannel.h"
#include "Logger.h"
#include "Windows.h"
#include "pcomm.h"
#include "crc16.h"
DCS_Channel::DCS_Channel():
    m_slave_addr(0),
    m_tx_cout(0),
    m_rx_cout(0)
{

}

bool DCS_Channel::open(const char* port, int baud, char parity, char databit, char stopbit)
{

    QString tmp = port;
    bool ok = false;
    if(tmp.length() < 3) {
        LOG_ERROR() << "error port" << tmp;
        return false;
    }
    tmp.remove(0,3);
    m_port_num = tmp.toInt(&ok);
    if(!ok){
        LOG_ERROR() << "error port" << tmp;
        return false;
    }

    m_port = port;
    sio_close(m_port_num);
    int err = sio_open(m_port_num);
    if(err != SIO_OK){
        LOG_ERROR() << "sio_open " << m_port_num << "failed";
        return false;
    }
    if(err != sio_ioctl(m_port_num,B19200,  P_NONE | BIT_8 | STOP_1 ))
    {
        LOG_ERROR() << "sio_ioctl failed";
        return false;
    }
    m_byte_timeout_ms = 20*1000/2400; // 20个字符的超时时间.
    return true;
}

bool DCS_Channel::close()
{
    sio_close(m_port_num);
    return true;
}

bool DCS_Channel::setByteTimeout(int us)
{
   // return (sio_SetReadTimeouts(m_port_num,1000000,us) == SIO_OK);
    return true;
}
//
bool DCS_Channel::set_response_timeout(int us)
{
    int ms = us / 1000;
    ms = (ms <=0)?1000:ms;

    return (sio_SetReadTimeouts(m_port_num,ms,m_byte_timeout_ms) == SIO_OK);

}

void DCS_Channel::setDeviceAddr(int _addr)
{
    m_slave_addr = _addr;

}
int  DCS_Channel::write_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;
    return 0;
}

void DCS_Channel::flush()
{
    sio_flush(m_port_num, 2);
}


int  DCS_Channel::write_register(int reg_addr, int value)
{
    m_tx_cout++;

    return 0;
}
int  DCS_Channel::send_then_recv(
                                 quint8 cmd,
                                 QByteArray &send_data,
                                 QByteArray &recv_data,
                                 int want
                                 )
{
    //sio_flush(m_port_num,0);
    send(cmd,send_data);
    QByteArray all_data;

    int res = recv(all_data,want);
    if(res <= 0)
    {
        LOG_ERROR() << "err: recv result=" << res;
        return 0;
    }

    LOG_DEBUG() <<"recv----<" <<  all_data.toHex();
    if(all_data.size() < 8){
        LOG_ERROR() << "err: send_then_recv count=" << all_data.size();
        return 0;
    }
    quint8 head = all_data[0];
    if(head != quint8(0xBB)) {
        LOG_ERROR("header = %02x",head);
        return -1;

    }
    quint8 s_h = all_data[1];
    quint8 s_l = all_data[2];

    int size = ( s_h << 8) + s_l;
    if((size+1) != all_data.size())
    {
        LOG_ERROR("err size = %d data=%d",size,all_data.size());
        return -2;
    }

    quint16 crc = ycm_crc16((uint8_t*)all_data.data(),all_data.size()-2);
    int index = all_data.size();
    quint8 crc_hi = (all_data[index-2]);
    quint8 crc_low = (all_data[index-1]);

    quint16 target_crc =( crc_hi << 8) + crc_low;
    if(crc != target_crc)
    {
        LOG_ERROR("crc err crc= %02x crc2=%02x hi=%02x low=%02x",crc,target_crc,crc_hi,crc_low);
        return -3;
    }
    recv_data.push_back(all_data.mid(4,all_data.size() - 6));
    return res;

}
#include <Windows.h>
//接收want字节，最大间隔字节超时时间time_us。
int  DCS_Channel::recv(QByteArray &data,quint32 want)
{
    char buffer[1024]={0,};
    if(want > 1024){
        LOG_ERROR()<< "want " << want << "must less than 1024";
        return -1;
    }
    //msleep(5);

    set_response_timeout(want*m_byte_timeout_ms);
    int res = sio_read(m_port_num,buffer,want<=0?1024:want);
    if(res > 0){

        data.append(buffer, res);
    }else{
        sio_flush(m_port_num,2);
    }

    return res;
}
int  DCS_Channel::send(quint8 cmd,QByteArray &data)
{
    QByteArray buf;
    buf.append(0xAA);
    //buf.append(0x0);
    //buf.append(0x0);
    buf.append(cmd);
    buf.append(m_slave_addr);
    buf.append(data);
    //包长度(2bytes) + 命令(1) + 称台编号(1) + 内容(N) + crc16(2)
    quint16 len = 6 + data.size(); // 从长度字段开始到结束的长度，包含CRC16

    buf.insert(1,(len>>8)&0xff);
    buf.insert(2,(len&0xff));

    quint16 crc = ycm_crc16((uint8_t*)buf.data() ,buf.size());

    buf.append(crc>>8);
    buf.append(crc&0xff);

    qDebug() << "send----->" << buf.toHex();
    return sio_write(m_port_num,buf.data(),buf.size());

}
int  DCS_Channel::read_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;

    return 0;
}
//cmd = 7
int  DCS_Channel::read_input_registers(int reg_addr, int nb,quint16* value)
{
    m_tx_cout+=nb;

    return 0;
}
QString DCS_Channel::port() const
{
    return m_port;
}

void DCS_Channel::get_rx_tx(int &rx, int &tx)
{

}

