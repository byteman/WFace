#ifndef MYCONFIG_H
#define MYCONFIG_H


class MyConfig
{
public:
    MyConfig();
    void SetSaveTime(int value);
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_save_time_min;
};

#endif // MYCONFIG_H
