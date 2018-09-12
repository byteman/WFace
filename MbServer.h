#ifndef MBSERVER_H
#define MBSERVER_H
#include <QThread>
#include "modbus.h"
#include "myconfig.h"
class MbServer:public QThread
{
    Q_OBJECT
public:
    MbServer();
    ~MbServer();
    bool StartServer(int port);
    bool StopServer();
    virtual void run();
    bool doWork();
    int Init(int port);
    void close_sigint(int dummy);
private:
    MyConfig *config;
    int m_port;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int server_socket;
    int master_socket;
    int fdmax;
    int rc;
    fd_set refset;
    fd_set rdset;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

};

#endif // MBSERVER_H
