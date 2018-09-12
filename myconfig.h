#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>
#include <QMap>
#include <QSettings>
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
enum CommuType{
    COMMU_ALL=0,
    COMMU_RTU,
    COMMU_TCP,
    COMMU_MAX
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
    bool SaveHostInfo(QString host, int port);
    bool SetModbusType(bool rtu);
    bool SaveUartsInfo(QStringList &ports);
    QString Unit();
    bool isAdmin();
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_save_time_min;
    int m_max_channel;
    int m_delay_ms;
    int m_port;
    int m_server_port;
    int m_commu_type; //通訊方式.
    //bool m_isRTU;
    QString m_host;
    QStringList m_port_names;
    bool m_admin;
    //AlarmInfoMap m_info_map;
    QString m_unit;
    QString m_title;
private:
    QSettings *config;
};

#endif // MYCONFIG_H
