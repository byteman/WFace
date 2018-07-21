#include "myconfig.h"
#include <QSettings>
#include <QDebug>
MyConfig::MyConfig():
    config(new QSettings("wface.ini",QSettings::IniFormat))
{
    //QSettings config("wface.ini", QSettings::IniFormat);

    config->setIniCodec("UTF-8");//添上这句就不会出现乱码了);

    m_poll_timeout       = config->value("/config/poll_timeout",30000).toInt();
    //if(m_poll_timeout < 10000) m_poll_timeout = 10000;
    m_scan_timeout       = config->value("/config/scan_timeout",100000).toInt();
    if(m_scan_timeout < 100000) m_scan_timeout = 100000;
    m_read_timeout       = config->value("/config/read_timeout",1000000).toInt();
    if(m_read_timeout < 500000) m_read_timeout = 500000;
    m_max_sample       = config->value("/config/max_save_sample",5000).toInt();
    if(m_max_sample < 500) m_max_sample = 500;
    m_save_time_min       = config->value("/config/save_time_min",5).toInt();
    if(m_max_sample > 24*60 ) m_max_sample = 24*60;
    m_unit       = config->value("/config/unit","kg").toString();
    m_admin       = config->value("/config/admin",false).toBool();
    m_title       = config->value("/config/title","Measure").toString();
//    m_alarm_index       = config->value("/alarm/condition",0).toInt();
//    if(m_alarm_index > 1) m_alarm_index = 0;
//    m_alarm_value = config->value("/alarm/value",100.0).toDouble();
    if(m_max_sample > 24*60 ) m_max_sample = 24*60;

    QString port = config->value("/config/port","").toString();
    m_port_names       = port.split(",",QString::SkipEmptyParts);
    qDebug() << port << "--------------" << m_port_names;
    int baud = config->value("/config/baud",115200).toInt();
    qDebug() << baud;
    m_max_channel = m_port_names.count();

    qDebug() << "title =" << m_title;
    qDebug() << "unit = " << m_unit;
}

void MyConfig::SetSaveTime(int value)
{
    m_save_time_min = value;
    //QSettings config("wface.ini", QSettings::IniFormat);
    config->setValue("/config/save_time_min",value);
}

void MyConfig::SaveUnit(QString unit)
{
    m_unit = unit;
    //QSettings config("wface.ini", QSettings::IniFormat);
    config->setValue("/config/unit",unit);
}
#if 0
void MyConfig::SaveAlarmSetting(int alarmIndex, double alarmValue)
{
    m_alarm_index = alarmIndex;
    m_alarm_value = alarmValue;
    QSettings config("wface.ini", QSettings::IniFormat);
    config->setValue("/alarm/condition",m_alarm_index);
    config->setValue("/alarm/value",m_alarm_value);
}
#endif
//保存某个地址的配置.
void MyConfig::SaveAlarmSetting(int addr, int alarmIndex, double alarmValue)
{
    //AlarmInfo info;
    //info.create(addr,alarmIndex,alarmValue);
    //m_info_map[addr] = info;
    //QSettings config("wface.ini", QSettings::IniFormat);
    config->setValue(QString("/alarm/cond%1").arg(addr), alarmIndex);
    config->setValue(QString("/alarm/value%1").arg(addr),alarmValue);

}
//读取某个地址的报警配置
bool MyConfig::GetAlarmSetting(int addr, AlarmInfo &aif)
{
    //QSettings config("wface.ini", QSettings::IniFormat);
    int index        = config->value(QString("/alarm/cond%1").arg(addr),1).toInt();
    double value     = config->value(QString("/alarm/value%1").arg(addr),10000).toDouble();
    aif.create(addr,index,value);
    return true;

}

QString MyConfig::Unit()
{
    return m_unit;
}

bool MyConfig::isAdmin()
{
    return m_admin;
}

