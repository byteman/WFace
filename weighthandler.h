#ifndef WEIGHTHANDLER_H
#define WEIGHTHANDLER_H

#include "cmdhandler.h"
class WeightHandler : public CmdHandler
{
     Q_OBJECT
public:
    WeightHandler(RTU_Modbus*  rtu);
    bool run();
    bool stop();
signals:
    void weightResult(int weight, quint16 state);
private:

};

#endif // WEIGHTHANDLER_H
