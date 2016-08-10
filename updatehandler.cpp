#include "updatehandler.h"
#include "modbus.h"
#include "PCOMM.H"
#include <qdebug.h>

static bool bRecv = false;
static int postion = 0;
static int total   = 0;
static UpdateHandler* gHander = NULL;
UpdateHandler::UpdateHandler(RTU_Modbus *rtu):
    _rtu(rtu),
    m_state(0)
{
    gHander =this;
}

int CALLBACK xCallback(long xmitlen, int buflen, char *buf, long flen)
{
/* Update receiving status */
    qDebug() << "send -> " << xmitlen << " total --> " <<flen;
    postion = xmitlen;
    total = flen;
    gHander->sendMessage(4,postion,total);
    return 0;
}

bool UpdateHandler::dorun()
{
    if(_rtu && m_state > 0)
    {
        if(m_state == 1)
        {
            //发送空格，等待回应.
            sio_write(m_port,"                   ",20);
            sendMessage(1,0,0);
            qDebug() << "send space";
        }
        else if(m_state == 2)
        {
            sendMessage(2,0,0);

        }
        else if(m_state == 3)
        {
            sendMessage(3,0,0);

        }
        else if(m_state == 4)
        {
            qDebug() << "send files 4";


        }
        if(bRecv)
        {
            char buffer[128] = {0,};
            sio_SetReadTimeouts(m_port,100,100);
            sio_read(m_port,buffer,128);
            QString rxStr(buffer);
            qDebug() << "recv:" << rxStr;

            if(m_state == 1)
            {
                if(rxStr.contains("return key.\r\n"))
                {
                    qDebug() << "get return key";
                    sio_write(m_port,"\r",1);
                    m_state = 2;
                }
            }
#if 1
            else if(m_state == 2)
            {
                if(rxStr.contains("MS:"))
                {
                    qDebug() << "send update";
                    sio_write(m_port,"update\r\n",8);
                    m_state = 3;
                }
            }
            else if(m_state == 3)
            {
                if(rxStr.contains("C"))
                {
                    qDebug() << tr("ready send file") << m_file;

                    int err = sio_FtYmodemTx(m_port,(char*)m_file.toStdString().c_str(),xCallback,27);
                    if(0 != err)
                    {
                        qDebug() << tr("start failed") << err;
                        sendMessage(-1,0,0);
                        sio_close(m_port);
                        m_state  = 0;

                    }
                    else
                    {
                        m_state = 4;

                    }

                }
            }
            else if(m_state == 4)
            {
                if(rxStr.contains("Programming Completed Successfully"))
                {
                    sendMessage(5,0,0);
                    qDebug() << "update ok";
                    sio_write(m_port,"reset\r\n",7);
                    sio_close(m_port);
                    m_state = 0;
                }
            }
#endif
            bRecv = false;

        }
        return true;
    }
    return false;
}
VOID CALLBACK CntIrq(int port)
{
    /* You should NOT read data at this point. */
    /* Send message to let main program to read data. */
    bRecv = true;
    qDebug() <<"cntIrq";
}
bool UpdateHandler::startUpdate(QString file)
{
    if(_rtu == NULL) return false;
#if 1
    QString port = _rtu->port();
    bool ok = false;
    m_port = port.right(1).toInt(&ok);
    if(ok==false) return false;
#else
    m_port = 3;
#endif
    if(sio_open(m_port) != 0)
    {
        MyException exc(tr("can not open serial port"));
        exc.raise();
    }
    if(sio_baud(m_port,9600)!=0)
    {
        MyException exc(tr("can not set serial port baud"));
        exc.raise();
    }
    m_file = file;
    sio_cnt_irq(m_port,CntIrq,1);
    m_state = 1;
    this->start();
    return true;
}

bool UpdateHandler::stop()
{
    m_state = 0;
    sio_close(m_port);
    this->wait();
    return true;
}

void UpdateHandler::sendMessage(int result, int pos, int total)
{
    emit updateResult(result,pos,total);
}

void UpdateHandler::run()
{

    while(dorun())
    {
        this->msleep(100);
    }
    sio_close(m_port);
}

MyException::MyException(QString error)
{
    setError(error);
}

QString MyException::getError() const
{
    return error;
}

void MyException::setError(const QString &value)
{
    error = value;
}
