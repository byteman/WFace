#ifndef MYCONFIG_H
#define MYCONFIG_H


class MyConfig
{
public:
    MyConfig();
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
};

#endif // MYCONFIG_H
