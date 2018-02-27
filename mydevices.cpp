#include "mydevices.h"

MyDevices::MyDevices(int max,QWidget* parent):
    QWidget(parent),
    m_row(4),m_col(8)
{
    for(int i = 0; i < max; i++ )
    {

    }
}


void MyDevices::clearAll()
{
    for(int i = 0; i < widgets.size();i++)
    {
        delete widgets[i];
    }
    widgets.clear();
}
void MyDevices::SetDeviceNum(int start, int num)
{
    clearAll();
    for(int i = start; i < (start+num); i++)
    {
        MyWidget* w = new MyWidget
    }
}

void MyDevices::DisplayWeight(int addr, int weight, quint16 state, quint16 dot)
{

}

