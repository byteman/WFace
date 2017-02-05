#include "cmdhandler.h"
#include <QDebug>



void CmdHandler::run()
{

    while ( !QThread::currentThread()->isInterruptionRequested() ) {
        if(doWork()) // 判断是否工作已经做完.做完就退出线程.
        {
            return;
        }
    }


}

bool CmdHandler::doWork()
{
    return true;
}
void CmdHandler::writeCmds()
{
    RegCmd cmd;
    while(cmdlist.size() > 0)
    {
        cmd = cmdlist.first();
        if(cmd.isRead)
        {
            if(cmd.reg_num == _rtu->read_input_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
            {

            }
        }
        else
        {
            if(cmd.reg_num != _rtu->write_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
            {
                qDebug() << "write failed";
            }
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

bool CmdHandler::startRun()
{
    this->start();
    return true;
}
bool CmdHandler::stop()
{
    this->requestInterruption();
    return true;
}
