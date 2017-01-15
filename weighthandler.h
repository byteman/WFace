#ifndef WEIGHTHANDLER_H
#define WEIGHTHANDLER_H

#include "cmdhandler.h"
#include <QList>



class WeightHandler : public CmdHandler
{
     Q_OBJECT
public:
    WeightHandler(RTU_Modbus*  rtu);
    void addCmd(RegCmd cmd);
    bool run();
    bool stop();
signals:
    void weightResult(int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
private:


};

#endif // WEIGHTHANDLER_H
