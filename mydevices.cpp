#include "mydevices.h"
#include <QGridLayout>
#include "utils.h"
MyDevices::MyDevices(int max,QGroupBox* parent):
    m_container(parent),
    m_max(36),
    m_row(6),m_col(6)
{
    for(int i = 0; i < max; i++ )
    {
        DevWidget* widget = new DevWidget(i+1,parent);
        widgets.push_back(widget);
        widget->hide();
    }
    //QGridLayout* layout = parent->layout();

    qlayout = new QGridLayout();

    parent->setLayout(qlayout);

}


void MyDevices::clearAll()
{
    for(int i = 0; i < widgets.size();i++)
    {
         widgets[i]->hide();
    }
}
void MyDevices::SetDeviceNum(int start, int num)
{
    clearAll();

    int width  = m_container->width();
    int height = m_container->height();

    int w  = width / m_col;
    int h  = height / m_row;

    for(int i = 0; i < num; i++ )
    {
        int row = i/m_col ;
        int col = i%m_col;
        widgets[i+start-1]->setGeometry(col*w,row*h,w,h);
        //qlayout->addWidget(widgets[i+start],row,col);
        widgets[i+start-1]->show();
    }
}

void MyDevices::DisplayWeight(int addr, int weight, quint16 state, quint16 dot)
{
    if(addr < widgets.size())
    {
        widgets[addr-1]->DisplayWeight(weight,state,dot);
    }
}

