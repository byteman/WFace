#ifndef CALIBHANDLER_H
#define CALIBHANDLER_H


#include "cmdhandler.h"
class CalibHandler : public CmdHandler
{
     Q_OBJECT
public:
    CalibHandler(RtuReader*  rtu);
    virtual bool doWork();
    bool readPara(int index=-1);
    bool stop();
    bool calibSet(int  index, qint32 weight,qint32 ad);
signals:
    void calibReadResult(int index, qint32 weight,qint32 ad);
    void calibProcessResult(int  index, int result);
private:
    bool m_set_calib_points[6];
    bool m_read_calib_points[6];
};
#endif // CALIBHANDLER_H
