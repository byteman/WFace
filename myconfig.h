#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>
#include <QMap>
class AlarmInfo{
public:
    AlarmInfo():
    enable(true),
    index(1),
    value(1000),
    addr(-1){

    }
    void create(int addr=-1)
    {
        this->enable = true;
        this->index = 1;
        this->addr = addr;
        this->value = 10000;
    }
    void create(int addr, int index, double value)
    {
        this->enable = true;
        this->index = index;
        this->addr = addr;
        this->value = value;
    }
    bool enable;
    int  index;
    int  addr;
    double value;
};
typedef QMap<int, AlarmInfo> AlarmInfoMap;
class MyConfig
{
public:
    MyConfig();
    void SetSaveTime(int value);
    void SaveUnit(QString unit);
    //void SaveAlarmSetting(int alarmIndex, double alarmValue);
    void SaveAlarmSetting(int addr, int alarmIndex, double alarmValue);
    bool GetAlarmSetting(int addr, AlarmInfo& aif);
    QString Unit();
    bool isAdmin();
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_save_time_min;
    //quint8 m_alarm_index; //0 小于等于设定值 1 大于设定值
    //float m_alarm_value; //报警值.
    bool m_admin;
    //AlarmInfoMap m_info_map;
    QString m_unit;
    QString m_title;
};

#endif // MYCONFIG_H
