#ifndef WEIGHTHANDLER_H
#define WEIGHTHANDLER_H

#include "cmdhandler.h"

#include <QList>



class WeightHandler : public CmdHandler
{
     Q_OBJECT
public:
    WeightHandler(ModbusReader*  rtu);


    virtual bool doWork();
    //去皮
    bool discardTare();
    //清零
    bool setZero();
    //放大10倍
    bool zoom10X();
    //皮重和净重切换
    bool changeGN();
    int  getDot();
    int changeChan();
    bool setHold(bool hold);
signals:
    void weightParaReadResult(quint16 div_high,quint16 div_low, quint32 full_high, quint32 full_low,int dot);
    void weightResult(int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
private:
    bool paraRead();
    bool WriteCtrlCmd(int reg, quint8 value);
    int  m_dot;
};

#endif // WEIGHTHANDLER_H
