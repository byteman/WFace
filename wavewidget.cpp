#include "wavewidget.h"
#include "qcustomchart.h"
WaveWidget::WaveWidget(QWidget *parent, int num):
    m_start(0),
    m_num(0)
{

    //m_chart = new QChartLineChart(parent,num);

    m_chart = new QCustomChart((QCustomPlot*)parent,num);
    //SetChannel(1,32);
    //m_num = num;
    //m_chart->SetFilter(new ShiftFilter(6));

}
//WaveWidget::WaveWidget(QCustomPlot* parent,int num)
//{
//    m_chart = new QCustomChart(parent,num);
//}
WaveWidget::~WaveWidget()
{

}

void WaveWidget::DisplayAllChannel(bool show)
{
    m_chart->DisplayAllChannel(show);
}

void WaveWidget::AppendData(int addr, float value)
{
    if(m_start == 0) return;
    int chan = addr - m_start;
    m_chart->AppendData(chan,value);
}

void WaveWidget::SetChannel(int start,int num)
{

    if((num == m_num) && (start==m_start)) return;
    m_start = start;
    m_num   = num;
    if(m_chart!=NULL){
        m_chart->SetChannel(start,num);
    }
}

void WaveWidget::DisplayChannel(int chan,bool bshow)
{
    m_chart->DisplayChannel(chan,bshow);

}

void WaveWidget::CloseAll()
{
    m_chart->CloseAll();
}

void WaveWidget::Clear()
{
    m_chart->Clear();
}
void WaveWidget::AppendDataArr(ChannelDataArray &wvd)
{
    //m_chart->AppendDataArray(wvd);
}

