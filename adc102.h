#ifndef ADC102_H
#define ADC102_H

#include <QObject>
#include "mymodbus.h"
#include <QTimer>
#include "cmdhandler.h"
#include <QList>
class ADC102 : public QObject
{
    Q_OBJECT
public:
    explicit ADC102(QObject *parent = 0);
    void setSlaveAddr(int addr);
signals:
    void scanResult(int type, int addr);
    void weightResult(int weight, quint16 state);
public slots:
    bool startScan(QString port,int baud, char parity, char databit, char stopbit);
    bool startReadWeight();
    bool stopReadWeight();
    bool stop();
    void onScanResult(int type,int addr);
    void onWeightResult(int weight, quint16 state);
    void timerHandler();
private:

    RTU_Modbus modbus;
    QTimer timer;
    CmdHandler* m_handler;

    QList<CmdHandler*> m_handlers;
    int m_interval;
    int m_slaveAddr;
};

#endif // ADC102_H
