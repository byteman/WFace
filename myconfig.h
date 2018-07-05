#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QString>
class MyConfig
{
public:
    MyConfig();
    bool SaveNetParam(QString ip, int port);
public:
    int m_poll_timeout;
    int m_scan_timeout;
    int m_read_timeout;
    int m_max_sample;
    int m_delay_ms;
    QString m_ipaddr;
    int m_port;
};

#endif // MYCONFIG_H
