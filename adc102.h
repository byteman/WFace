#ifndef ADC102_H
#define ADC102_H

#include <QObject>
#include "mymodbus.h"
#include <QTimer>
#include "cmdhandler.h"
#include "parahandler.h"
#include <QList>
class ADC102 : public QObject
{
    Q_OBJECT
public:
    explicit ADC102(QObject *parent = 0);
    void setSlaveAddr(int addr);
    bool hasConnect();
signals:
    void scanResult(int type, int addr);
    void weightResult(int weight, quint16 state);
    void paraReadResult(Para _para);
    void calibProcessResult(int index, int result);
    void calibPointResult(int index, int weight, int ad);
public slots:
    bool discardTare();
    bool setZero();
    bool zoom10X();

    bool paraSave(Para _para);
    bool startScan(QString port,int baud, char parity, char databit, char stopbit);
    bool startReadWeight();
    bool startReadPara();
    bool stopReadPara();

    bool startCalib(int index, int weight);
    bool readCalibPoints(int index = -1);
    bool stopReadWeight();
    bool stop();
    void onCalibProcessResult(int index, int result);
    void onCalibPointResult(int index, int weight, int ad);
    void onParaReadResult(Para _para);
    void onScanResult(int type,int addr);
    void onWeightResult(int weight, quint16 state);
    void timerHandler();
private:

    RTU_Modbus modbus;
    QTimer timer;
    CmdHandler* m_handler;
    bool m_connect;
    QList<CmdHandler*> m_handlers;
    int m_interval;
    int m_slaveAddr;
    Para m_para;
};

#endif // ADC102_H
