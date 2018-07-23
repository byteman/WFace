#include "WorkerWidgetManager.h"
#include "WeightWidget.h"

WorkerWidgetManager::WorkerWidgetManager(QObject *parent) : QObject(parent)
{
    //new WeightHandler(&reader);
    //Reader 是通讯配置后，创建的.
    //m_widgets["weight"] = scaner;

//    weight = new WeightWidget(&reader);

//    calib = new CalibHandler(&reader);
//    para = new ParaHandler(&reader);
//    corn = new CornHandler(&reader);
//    poller = new PollerHandler(&reader);

}

void WorkerWidgetManager::changeWidget(QString name, bool start)
{

}

void WorkerWidgetManager::selectBus(QString name)
{

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

