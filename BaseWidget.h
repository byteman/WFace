#ifndef BASEWIDGET_H
#define BASEWIDGET_H
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "myconfig.h"
#include "utils.h"
#include "adc102.h"
//每个都是一个单独的功能页面.
class BaseWidget:public QWidget
{
     Q_OBJECT
public:
    BaseWidget(QWidget* parent=0);
    virtual ~BaseWidget();
    MyConfig& config();
    virtual void hidenForGuest(void);
    void traversalControl(const QObjectList &q);
protected:
    MyConfig cfg;
};

#endif // BASEWIDGET_H
