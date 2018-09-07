#ifndef PARAHANDLER_H
#define PARAHANDLER_H

#include "cmdhandler.h"
class Para
{
public:
    quint16 dot; //小数点位数 0 - 4
    quint16 div_high; //分度值[高位量程] 1，2，5，10，20，50，100。
    quint16 div_low; //分度值[低位量程] 1，2，5，10，20，50，100。
    quint32 span_high; //满量程（高位量程）
    quint32 span_low;   //中间量程（低位量程）
    quint16 unit; //单位 0：kg；1：g；2：t
    quint16 pwr_zero_span;//开机清零范围（0～100）：0%～100%
    quint16 hand_zero_span;//手动清零范围（0～100）：0%～100%
    quint16 zero_track_span;//零点跟踪范围（0～99）：0:OFF；1:0.1d/s … 98:9.8d/s；99:9.9d/s。
    quint16 stable_span; //稳定显示范围（0～99）:0:OFF；1:0.1d … 98:9.8d；99:9.9d。
    quint16 filter_level;//滤波等级:（0～5）数值越大滤波越重
    quint32 sensor_full_span; // 传感器总量程（所有传感器量程和）
    quint32 sensor_mv; //传感器平均灵敏度，200000 代表 2.00000mV/V
    quint16 slave_addr; //从机地址（自己的地址）：1～32
    quint16 version; //万位是产品编号，百位和千位是发行版本号，个位和十位是临时版本号
    quint16 adRate; //AD采样率.
    quint32 serial; //唯一序列号.
};
class ParaHandler : public CmdHandler
{
     Q_OBJECT
public:
    ParaHandler(ModbusReader*  rtu);
    int getDot();

signals:
    void paraReadResult(Para _para);
    void paraWriteResult(int result);
public slots:

    bool paraSave(Para _para);
    bool paraRead(Para &_para);
public:
    virtual bool doWork();

private:
    bool _paraSave(Para &_para,int &reg);
    bool m_write;
    Para m_para;
};

#endif // PARAHANDLER_H
