#include "timerworker.h"
#include "cmdhandler.h"
#include <QTimer>
TimerWorker::TimerWorker(QObject *parent) : QObject(parent),
    m_handler(NULL)
{

}

void TimerWorker::doWork()
{
       // m_mutex.lock();
       if(m_handler != NULL)
       {
           if(m_handler->myrun())
           {
               Start(m_interval,m_handler);
           }
           else
           {
               m_handler = NULL;
           }

       }
       //m_mutex.unlock();

}

void TimerWorker::Start(int interval, CmdHandler *handler)
{
      //m_mutex.lock();
    m_handler = handler;
    m_interval = interval;
    QTimer::singleShot(interval,this,SLOT(doWork()));
//m_mutex.unlock();
}

