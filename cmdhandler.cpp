#include "cmdhandler.h"
#include <QDebug>


bool CmdHandler::stop()
{
    return true;
}

bool CmdHandler::myrun()
{
    return true;
}
bool CmdHandler::writeCmds()
{
    RegCmd cmd;
    bool err = cmdlist.size()>0;

    while(cmdlist.size() > 0)
    {
        cmd = cmdlist.first();
        qDebug() << "read " << cmd.reg_addr;
        if(cmd.reg_num != _rtu->write_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
        {
            qDebug() << "write failed";
            if(cmd.retry_num > 0)
            {
                cmd.retry_num--;
            }
            err = false;
        }

        cmdlist.pop_front();
    }
    return err;

}

bool CmdHandler::addCmd(RegCmd cmd)
{
    //if(cmdlist.size() > 5)
    //    return false;
    cmdlist.push_back(cmd);
    return true;
}

bool CmdHandler::addCmd(int reg_addr, int reg_num, quint16 *reg_values)
{
    RegCmd cmd;
    cmd.reg_addr = reg_addr;
    cmd.reg_num = reg_num;
    for(int i = 0; i < reg_num; i++)
    {
        cmd.reg_value[i] = reg_values[i];
    }
    return addCmd(cmd);
}
