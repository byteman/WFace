#include "wavewidget.h"
#include "qcustomchart.h"
WaveWidget::WaveWidget(QWidget *parent, int num)
{

    //m_chart = new QChartLineChart(parent,num);

    m_chart = new QCustomChart((QCustomPlot*)parent,num);
    m_num = num;
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
    m_chart->AppendData(addr,value);
}

void WaveWidget::SetChannel(int start,int num)
{
    if(num == m_num) return;
    if(m_chart!=NULL){
        m_chart->SetChannel(num);
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

