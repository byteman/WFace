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
    bool start();
    bool stop();
    bool calib(int index);
    bool paraRead();
signals:
    void chanADReadResult(QList<qint32> chanAD);
    void chanKReadResult(QList<float> chanK);
private:


};
#endif // CALIBHANDLER_H
