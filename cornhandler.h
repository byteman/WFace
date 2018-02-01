#ifndef CORN_HANDLER_H
#define CORN_HANDLER_H


#include "cmdhandler.h"

class CornHandler : public CmdHandler
{
     Q_OBJECT
public:
    CornHandler(RtuReader*  rtu);
    virtual bool doWork();
    bool init();
    bool startCalib();
    bool stopCalib();
    bool calib(int index);

    bool ReadParam();
    bool setK(int index,float k);
    bool setKs(QList<float> ks);
    bool setSensorNum(quint16 num);
    int  getSensorNum();

signals:

    void chanADReadResult(QList<float> chanAD);
    void chanKReadResult(int num,QList<float> chanK);
private:
    bool paraRead();
    int  m_sensor;


};
#endif // CALIBHANDLER_H
