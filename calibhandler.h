#ifndef CALIBHANDLER_H
#define CALIBHANDLER_H


#include "cmdhandler.h"
#include "Poco/Timer.h"
#include <vector>
struct Sensor
{
    Sensor()
    {
        valid = false;
    }
    int     addr;
    float   k;
    bool    valid;
    float   zero;
    qint32  mv;
    qint32  wt;
};
#define MAX_SENSOR_NUM 6

class CalibHandler : public CmdHandler
{
     Q_OBJECT
public:
    CalibHandler(RTU_Modbus*  rtu);
    bool myrun();
    bool readPara(int index=-1);
    bool stop();
    bool calibSet(bool hand,int  index, qint32 weight,qint32 ad);
    bool calibSetAll(std::vector<int> weights,bool hand);
    bool calibZeroSet(int  index=-1);
    bool calibAllZero(int num);
    bool modifyKs(std::vector<qint32> ks);
    bool readSensorNum();
    bool readRtParas(int num);
    void run();
    void onTimer(Poco::Timer &timer);
signals:
    void calibReadResult(Sensor* sensors,int num);
    void calibProcessResult(int  index, int result);
private:
    bool m_set_calib_points[6];
    bool m_read_calib_points[6];
    bool m_read_sensor_num;
    int  m_sensor_num;
    Sensor sensors[MAX_SENSOR_NUM];
    volatile bool _run;
    Poco::Timer _timer;

};
#endif // CALIBHANDLER_H
