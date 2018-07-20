#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <QObject>
#include "devwidget.h"
#include <QVector>
#include <QGroupBox>
#include <QGridLayout>
#include <QMap>
#include "wavefile.h"
#include "csvfile.h"
class MyDevices:public QObject
{
    Q_OBJECT
public:
    MyDevices(int max=32,QGroupBox* parent=NULL );
    void SetDeviceNum(int  start, int num);
    void SetMaxSampleNum(int max);
    void Timeout(int addr);
    void SetUnit(QString unit);
    void SetAlarmSetting(int addr,int alarmSetting, double value);
    void DisplayWeight(int addr,int weight, quint16 state,quint16 dot);
    void SaveWave();
    void LoadWave(QString file,ChannelsData& datas);
    void GetNum(int &start,int &num);
    int  GetEndAddr();
    void listWaveFiles(QStringList &files);
signals:
    void WaveFull();
private:
    void AppendWave(int addr,float value);
    QVector<DevWidget*> widgets;
    void clearAll();
    int m_row,m_col;
    int m_max;
    int m_max_sample;
    QGroupBox* m_container;
    QGridLayout* qlayout;
    QMap<int,QByteArray > m_values;

    CSVFile m_csv;
    int m_num;
    int m_start;
    int m_end;
    int m_index;
    QString m_unit;
    QString CreateDir(QString type);
    QString GetFileName();
    void AppendItem(QString &output, QString item);

    void GetFileName(QString &csvFile, QString &waveFile);
};

#endif // MYDEVICES_H
