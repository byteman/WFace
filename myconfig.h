#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>
class MyConfig
{
public:
    MyConfig();
    void SetSaveTime(int value);
    void SaveUnit(QString unit);
    void SaveAlarmSetting(int alarmIndex, double alarmValue);
    QString Unit();
    bool isAdmin();
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_save_time_min;
    quint8 m_alarm_index; //0 小于等于设定值 1 大于设定值
    float m_alarm_value; //报警值.
    bool m_admin;

    QString m_unit;
    QString m_title;
};

#endif // MYCONFIG_H
