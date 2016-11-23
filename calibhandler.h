#ifndef CALIBHANDLER_H
#define CALIBHANDLER_H


#include "cmdhandler.h"
class CalibHandler : public CmdHandler
{
     Q_OBJECT
public:
    CalibHandler(RTU_Modbus*  rtu);
    bool run();
    bool readPara(int index=-1);
    bool stop();
    bool calibSet(bool hand,int  index, qint32 weight,qint32 ad);
    bool calibZeroSet(int  index);
signals:
    void calibReadResult(int index, qint32 weight,qint32 ad);
    void calibProcessResult(int  index, int result);
private:
    bool m_set_calib_points[6];
    bool m_read_calib_points[6];
};
#endif // CALIBHANDLER_H
