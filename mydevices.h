#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <QObject>
#include "mywidget.h"
#include <QList>
class MyDevices:public QObject
{
    Q_OBJECT
public:
    MyDevices(int max=32,QWidget* parent );
    void SetDeviceNum(int  start, int num);
    void DisplayWeight(int addr,int weight, quint16 state,quint16 dot);
private:
    QList<MyWidget*> widgets;
    void clearAll();
    int m_row,m_col;
};

#endif // MYDEVICES_H
