#ifndef WORKERWIDGETMANAGER_H
#define WORKERWIDGETMANAGER_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QMap>
#include <QGroupBox>
#include "BaseWidget.h"
#include "readerwidget.h"
#include "mydevices.h"
/**
 * @brief The WorkerWidgetManager class
 * 管理所有的BaseWidget，负责创建和切换.
 */
class WorkerWidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit WorkerWidgetManager(QObject *parent = 0);
    void changeWidget(QString name,bool start);
    void setChannelUI(QGroupBox* gbox);
    void selectBus(int index);
    RtuReader* getReader();
    static WorkerWidgetManager* get();
signals:

public slots:

private:
    //QList<BaseWidget*> m_widgets;
    QMap<QString,BaseWidget*> m_widgets;
    IReaderWidget* m_reader;
    QTabWidget* m_container;
    QMap<QString,IReaderWidget*> m_channels;
    QGroupBox *m_grp;
    //MyDevices* devices;

};

#endif // WORKERWIDGETMANAGER_H
