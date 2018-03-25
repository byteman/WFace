#include "myconfig.h"
#include <QSettings>
MyConfig::MyConfig()
{
    QSettings config("wface.ini", QSettings::IniFormat);

    m_poll_timeout       = config.value("/config/poll_timeout",30000).toInt();
    if(m_poll_timeout < 30000) m_poll_timeout = 30000;
    m_scan_timeout       = config.value("/config/scan_timeout",100000).toInt();
    if(m_scan_timeout < 100000) m_scan_timeout = 100000;
    m_read_timeout       = config.value("/config/read_timeout",1000000).toInt();
    if(m_read_timeout < 500000) m_read_timeout = 500000;
    m_max_sample       = config.value("/config/max_save_sample",5000).toInt();
    if(m_max_sample < 500) m_max_sample = 500;



}

