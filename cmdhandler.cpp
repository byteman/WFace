#include "cmdhandler.h"
#include <QDebug>



void CmdHandler::run()
{

    while ( !QThread::currentThread()->isInterruptionRequested() ) {
        if(doWork()) // 判断是否工作已经做完.做完就退出线程.
        {
            return;
        }
        processCmds();
    }


}

bool CmdHandler::doWork()
{
    return true;
}

bool CmdHandler::init()
{
    return true;
}
void CmdHandler::processCmds()
{
    RegCmd cmd;
    while(cmdlist.size() > 0)
    {
        cmd = cmdlist.first();
        cmd.error = REG_ERROR_OK;
        if(cmd.isRead)
        {
            if(cmd.reg_num == _rtu->read_input_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
            {
                cmd.error = REG_ERROR_TIMEOUT;
            }
        }
        else
        {
            if(cmd.reg_num != _rtu->write_registers(cmd.reg_addr,cmd.reg_num,cmd.reg_value))
            {
                QString err= QString("write reg[%1] num=[%2] failed").arg(cmd.reg_addr).arg(cmd.reg_num);

                qDebug() << err;
                cmd.error = REG_ERROR_TIMEOUT;
            }
        }
        emit OperationResult(cmd);
        cmdlist.pop_front();
    }


}

bool CmdHandler::postCmd(RegCmd cmd)
{
    if(cmdlist.size() > 5)
        return false;
    cmdlist.push_back(cmd);
    return true;
}

bool CmdHandler::postWriteRegs(int reg_addr, int reg_num, quint16 *values)
{
    RegCmd cmd;
    cmd.reg_addr = reg_addr;
    cmd.reg_num  = reg_num;
    if(reg_num > 32) return false;
    for(int i = 0; i < reg_num; i++)
    {
        cmd.reg_value[i] = values[i];
    }
    return postCmd(cmd);
}

bool CmdHandler::postReadRegs(int reg_addr, int reg_num)
{
    RegCmd cmd;
    cmd.isRead = true;
    cmd.reg_addr = reg_addr;
    cmd.reg_num  = reg_num;
    return postCmd(cmd);
}

bool CmdHandler::startRun()
{
    bInit = false;
    init();
    this->start();
    return true;
}
bool CmdHandler::stop()
{
    this->requestInterruption();
    return this->wait(3000);
}
