#ifndef CALIBHANDLER_H
#define CALIBHANDLER_H


#include "cmdhandler.h"

class CalibHandler : public CmdHandler
{
     Q_OBJECT
public:
    CalibHandler(RtuReader*  rtu);
    virtual bool doWork();
    bool init();
    bool readPara(int index=-1);
    bool calibZero(quint8 addr);
    bool calibWgt(quint8 addr);
    bool calibSet(int  index, float weight,qint32 ad);
    bool clearZero(quint8 addr);
    bool queryErrorSensor();
    bool queryChangeSensor();
    bool querySensorWgt(quint8 addr);
    bool savePara(quint32 full,quint32 mv);
    int getDot(){return m_dot;}
signals:
    void calibReadResult(int index, qint32 weight,qint32 ad,int dot);
    void calibProcessResult(int  index, int result);
    void calibParaResult(quint32 sensorMv, quint32 sensorFull);
    void chanADReadResult(QList<float> chanAD);
private:
    bool m_set_calib_points[6];
    bool m_read_calib_points[6];
    bool paraRead();
    int m_dot;
    int m_sensor;

};
#endif // CALIBHANDLER_H
