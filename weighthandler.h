#ifndef WEIGHTHANDLER_H
#define WEIGHTHANDLER_H

#include "cmdhandler.h"
#include <QList>



class WeightHandler : public CmdHandler
{
     Q_OBJECT
public:
    WeightHandler(RtuReader*  rtu);
    void addCmd(RegCmd cmd);
    void run();
    bool stop();
signals:
    void weightResult(int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
private:


};

#endif // WEIGHTHANDLER_H
