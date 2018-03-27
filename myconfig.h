#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>
class MyConfig
{
public:
    MyConfig();
    void SetSaveTime(int value);
    void SaveUnit(QString unit);
    QString Unit();
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_save_time_min;

    QString m_unit;
};

#endif // MYCONFIG_H
