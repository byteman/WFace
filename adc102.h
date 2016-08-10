#ifndef ADC102_H
#define ADC102_H

#include <QObject>
#include "mymodbus.h"
#include <QTimer>
#include "cmdhandler.h"
#include "parahandler.h"
#include <QList>
#include "updatehandler.h"
class ADC102 : public QObject
{
    Q_OBJECT
public:
    explicit ADC102(QObject *parent = 0);
    bool setSlaveAddr(int addr);
    bool hasConnect();
signals:
    void updateResult(int result, int pos, int total);
    void scanResult(int type, int addr);
    void weightResult(int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
    void paraReadResult(Para _para);
    void calibProcessResult(int index, int result);
    void calibPointResult(int index, int weight, int ad);
public slots:
    bool discardTare();
    bool setZero();
    bool zoom10X();
    bool changeGN();
    bool paraSave(Para _para);
    bool startScan(QString port,int baud, char parity, char databit, char stopbit,bool findOne=true);
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
    void onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross, qint32 tare);
    void onUpdateResult(int result, int pos, int total);
    void timerHandler();
    bool modifyAddr(quint16 oldAddr, quint16 newAddr);
    bool reset();
    bool startUpdate(QString file);
private:
    UpdateHandler* handler_update;
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
