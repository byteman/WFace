#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <QObject>
#include "devwidget.h"
#include <QVector>
#include <QGroupBox>
#include <QGridLayout>
class MyDevices:public QObject
{
    Q_OBJECT
public:
    MyDevices(int max=32,QGroupBox* parent=NULL );
    void SetDeviceNum(int  start, int num);
    void Timeout(int addr);
    void DisplayWeight(int addr,int weight, quint16 state,quint16 dot);
private:
    QVector<DevWidget*> widgets;
    void clearAll();
    int m_row,m_col;
    int m_max;
    QGroupBox* m_container;
    QGridLayout* qlayout;
};

#endif // MYDEVICES_H