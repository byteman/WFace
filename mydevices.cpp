#include "mydevices.h"
#include <QGridLayout>
#include "utils.h"
#include <QDir>
#include <QDateTime>
MyDevices::MyDevices(int max,QGroupBox* parent):
    m_container(parent),
    m_max(36),
    m_row(6),m_col(6),
    m_num(1),m_max_sample(5000)
{
    for(int i = 0; i < max; i++ )
    {
        DevWidget* widget = new DevWidget(i,parent);
        connect(widget,SIGNAL(onDoubleClick(int,bool)),this,SLOT(onDoubleClick(int,bool)));
        widgets.push_back(widget);
        widget->hide();
    }
}
//创建目录失败
bool MyDevices::SetWaveDir(QString path)
{
    //创建目录看能否成功
    QDir dir(path);
    if(!dir.exists())
    {
        if(!dir.mkpath(path)){
            m_wave_dir=QDir::currentPath();
            return false;
        }
    }
    m_wave_dir = path;
    return true;
}
void MyDevices::zoomDevice(int addr)
{
    clearAll();
    int w  = m_container->width();
    int h = m_container->height();

    widgets[addr]->SetUnit(m_unit);
    widgets[addr]->setGeometry(0,0,w,h);
    //qlayout->addWidget(widgets[i+start],row,col);
    widgets[addr]->Show();
}
void MyDevices::onDoubleClick(int addr,bool zoom)
{

    if(zoom){
       zoomDevice(addr);
    }else{

        SetDeviceNum(m_start,m_num);
    }


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
        widgets[i+start]->Reset();
        widgets[i+start]->SetUnit(m_unit);
        widgets[i+start]->setGeometry(col*w,row*h,w,h);
        //qlayout->addWidget(widgets[i+start],row,col);
        widgets[i+start]->Show();
    }
    m_num = num;
    m_start = start;
    m_end = start+num ;
    m_values.clear();
    m_csv.Clear();
}

void MyDevices::SetMaxSampleNum(int max)
{
    m_max_sample = max;
}

void MyDevices::Timeout(int addr)
{
    if(addr < widgets.size())
    {
        widgets[addr]->Timeout();
        m_csv.Append(addr, "",m_end,false);
    }
}

void MyDevices::SetUnit(QString unit)
{
    m_unit = unit;
    for(int i = 0; i < widgets.size();i++)
    {
        widgets[i]->SetUnit(unit);
    }
}

void MyDevices::SetAlarmSetting(int addr,int alarmSetting, double value)
{
    //for(int i = 0; i < widgets.size();i++)
    //{
    if(addr < widgets.size() && addr >= 0)
    {
        widgets[addr]->SetAlarmSetting(alarmSetting,value);
    }

    //}
}

void MyDevices::DisplayWeight(int addr, int weight, quint16 state, quint16 dot)
{
    if(addr < widgets.size())
    {

        QString str = widgets[addr]->DisplayWeight(weight,state,dot);
        m_csv.Append(addr, str,m_end,false);
        AppendWave(addr, utils::int2float(weight,dot));
    }
}
//获取保存的目录
QString MyDevices::CreateDir(QString type)
{

    QString target_dir=QString("%1/%2").arg(m_wave_dir).arg(type);
    QDir dir(target_dir);
    if(!dir.exists())
    {
        if(!dir.mkpath(target_dir)){

        }
    }
    return target_dir;
}
void MyDevices::GetFileName(QString &csvFile,QString &waveFile)
{
    QString dt  = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString wave_dir = CreateDir("wave");
    QString csv_dir = CreateDir("csv");
    waveFile = QString("%1/%2.wave").arg(wave_dir).arg(dt);
    csvFile = QString("%1/%2.csv").arg(csv_dir).arg(dt);

}
#include "wavefile.h"
void MyDevices::SaveWave()
{
    QString wave,csv;
    GetFileName(csv,wave);
    WaveFile wv(wave);
    //CSVFile  csv(fname);
    QMapIterator<int, QByteArray> i(m_values);
    while (i.hasNext()) {
        i.next();
        wv.writeChan(i.key(),m_values[i.key()]);
        //cout << i.key() << ": " << i.value() << endl;
    }
    m_csv.SaveFile(csv);
    m_values.clear();
}

void MyDevices::LoadWave(QString file, ChannelsData &datas)
{
    QString filename = CreateDir("wave") + "/" + file;

    WaveFile wvf;
    wvf.load(filename,datas);

}

void MyDevices::GetNum(int &start, int &num)
{
    start = m_start;
    num = m_num;
}

int MyDevices::GetEndAddr()
{
    return m_end -1;
}

void MyDevices::listWaveFiles(QStringList &files)
{
    QString dir = CreateDir("wave");
    QDir wvDir(dir);
    files = wvDir.entryList(QDir::Files);
}

void MyDevices::AppendWave(int addr, float value)
{
    m_values[addr].append((const char*)&value,sizeof(float));
    if(m_values[addr].size() > m_max_sample)
    {
        emit WaveFull();
    }

}

