#ifndef PARAHANDLER_H
#define PARAHANDLER_H

#include "cmdhandler.h"
class Para
{
public:
    quint16 sensor_num; //小数点位数 0 - 4
    quint16 read_time_out; //分度值[高位量程] 1，2，5，10，20，50，100。
    quint16 limit; //分度值[低位量程] 1，2，5，10，20，50，100。
    int result; // 读取成功失败与否 0 成功 1读取超时.
    QByteArray toByteArray()
    {
        QByteArray out;
        out.append(sensor_num>>8);
        out.append(sensor_num&0xff);
        out.append(read_time_out>>8);
        out.append(read_time_out&0xff);
        out.append(limit>>8);
        out.append(limit&0xff);
        return out;
    }

};
#include "command.h"

class ParaHandler : public CmdHandler
{
     Q_OBJECT
public:
    ParaHandler(RtuReader*  rtu);
    int getDot();

signals:
    void paraReadResult(Para _para);
    void paraWriteResult(int result);
public slots:

    bool paraSave(Para _para);
    bool paraRead(Para &_para);
public:
    virtual bool doWork();

    bool queryErrorSensor(SensorErrInfoList& list);
    bool queryChangeSensor(SensorWgtInfoList& list);
    bool modifyAddr(quint8 addr);

private:
    bool _paraSave(Para &_para,int &reg);
    bool m_write;
    Para m_para;
    quint16 getU16(QByteArray &buf, int index);
};

#endif // PARAHANDLER_H
