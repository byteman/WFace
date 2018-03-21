#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QObject>
#include <QVector>
//#include "qwtchannel.h"
#include "ilinechart.h"
typedef QVector<float> ChannelData;
typedef QVector<ChannelData> ChannelDataArray;

class WaveWidget:public QObject
{
    Q_OBJECT
public:

    WaveWidget(QWidget* parent,int num=1);
    //WaveWidget(QCustomPlot* parent,int num=1);
    ~WaveWidget();
    void DisplayChannel(int chan=0,bool bshow=false);
    void DisplayAllChannel(bool show);
    void AppendDataArr(ChannelDataArray &wvd);
    void AppendData(int addr, float value);
    void SetChannel(int start,int num);
    void CloseAll();
    void Clear();
private:

    int m_num;

    ILineChart* m_chart;


};

#endif // WAVEWIDGET_H
