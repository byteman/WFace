#include "myconfig.h"
#include <QSettings>
#include <QDebug>
MyConfig::MyConfig()
{
    QSettings config("wface.ini", QSettings::IniFormat);

    m_poll_timeout       = config.value("/config/poll_timeout",30000).toInt();
    //if(m_poll_timeout < 10000) m_poll_timeout = 10000;
    m_scan_timeout       = config.value("/config/scan_timeout",100000).toInt();
    if(m_scan_timeout < 100000) m_scan_timeout = 100000;
    m_read_timeout       = config.value("/config/read_timeout",1000000).toInt();
    if(m_read_timeout < 500000) m_read_timeout = 500000;
    m_max_sample       = config.value("/config/max_save_sample",5000).toInt();
    if(m_max_sample < 500) m_max_sample = 500;
    m_save_time_min       = config.value("/config/save_time_min",5).toInt();
    if(m_max_sample > 24*60 ) m_max_sample = 24*60;
    m_unit       = config.value("/config/unit","kg").toString();
    m_admin       = config.value("/config/admin",false).toBool();
    m_title       = config.value("/config/title","Measure").toString();
    qDebug() << "title =" << m_title;
}

void MyConfig::SetSaveTime(int value)
{
    m_save_time_min = value;
    QSettings config("wface.ini", QSettings::IniFormat);
    config.setValue("/config/save_time_min",value);
}

void MyConfig::SaveUnit(QString unit)
{
    m_unit = unit;
    QSettings config("wface.ini", QSettings::IniFormat);
    config.setValue("/config/unit",unit);
}

QString MyConfig::Unit()
{
    return m_unit;
}

bool MyConfig::isAdmin()
{
    return m_admin;
}

