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
enum TimeUnit{
    TU_MIN=0, //分钟
    TU_HOUR=1 //小时
};
typedef QMap<int, AlarmInfo> AlarmInfoMap;
class MyConfig
{
public:
    MyConfig();
    void SetSaveTime(int value);
    void SetSaveTimeUnit(TimeUnit timeunit);
    void SaveUnit(QString unit);
    //void SaveAlarmSetting(int alarmIndex, double alarmValue);
    void SaveAlarmSetting(int addr, int alarmIndex, double alarmValue);
    bool GetAlarmSetting(int addr, AlarmInfo& aif);
    bool SaveHostInfo(QString host, int port);
    bool SetModbusType(bool rtu);
    bool SaveUartsInfo(QStringList &ports);
    bool IsModulesEnable(QString name);
    bool SaveWaveDir(QString dir);

    QString Unit();
    bool isAdmin();
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_save_time_min;
    int m_save_time;
    TimeUnit m_time_unit; //时间单位.
    int m_max_channel;
    int m_delay_ms;
    int m_port;
    int m_server_port;
    int m_commu_type; //通訊方式.
    int m_default_addr; //多串口模式下,读取的默认设备地址.
    //bool m_isRTU;
    QString m_host;
    QStringList m_port_names;
    QString m_wave_dir;

    bool m_uart_auto;
    bool m_admin;
    bool m_is_debug;
    //AlarmInfoMap m_info_map;
    QString m_unit;
    QString m_title;
    QMap<QString,bool> m_modules;
    QStringList m_params;
    QString m_lang;//语言
private:
    QSettings *config;
};

#endif // MYCONFIG_H
