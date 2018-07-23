#include "WorkerWidgetManager.h"
#include "WeightWidget.h"
#include "UartSettingWidget.h"
#include "TcpSettingWidget.h"
#include "QComSettingWidget.h"
WorkerWidgetManager::WorkerWidgetManager(QObject *parent) : QObject(parent),
    m_reader(NULL)
{

}

void WorkerWidgetManager::changeWidget(QString name, bool start)
{

}

void WorkerWidgetManager::setChannelUI(QGroupBox *gbox)
{
    m_grp = gbox;
}

void WorkerWidgetManager::selectBus(int index)
{


    if(m_reader != NULL){
        m_grp->layout()->removeWidget(m_reader);
        m_reader->deleteLater();
    }
    if(index == 0){
        m_reader = new UartSettingWidget();
        m_grp->layout()->addWidget(m_reader);


    }else  if(index == 2){
        m_reader = new TcpSettingWidget();
        m_grp->layout()->addWidget(m_reader);

    }else if(index == 1){
        m_reader = new QComSettingWidget();
        m_grp->layout()->addWidget(m_reader);
    }

}

RtuReader* WorkerWidgetManager::getReader()
{
    return m_reader->GetReader();
}
//Q_GLOBAL_STATIC(WorkerWidgetManager, workerManger)


WorkerWidgetManager *WorkerWidgetManager::get()
{
    static WorkerWidgetManager wwm;
    return &wwm;
}

