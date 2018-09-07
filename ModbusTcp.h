#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include "ModbusChannel.h"

class ModbusTcp:public ModbusChannel
{
public:
    ModbusTcp();
    virtual void sleep();
    bool open(const char* host,int port);
private:
    QString m_port;
};

#endif // MODBUSTCP_H
