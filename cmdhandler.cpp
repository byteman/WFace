#include "cmdhandler.h"
#include <QDebug>


bool CmdHandler::stop()
{
    return true;
}

bool CmdHandler::run()
{
    return true;
}
void CmdHandler::writeCmds()
{
    RegCmd cmd;
    while(cmdlist.size() > 0)
    {
        cmd = cmdlist.first();
        if(cmd.reg_num != _rtu->write_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
        {
            qDebug() << "write failed";
        }

        cmdlist.pop_front();
    }


}

bool CmdHandler::addCmd(RegCmd cmd)
{
    if(cmdlist.size() > 5)
        return false;
    cmdlist.push_back(cmd);
    return true;
}
