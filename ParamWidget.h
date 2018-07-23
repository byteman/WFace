#ifndef PARAMWIDGET_H
#define PARAMWIDGET_H

#include <QWidget>
#include "BaseWidget.h"
#include "parahandler.h"
namespace Ui {
class ParamWidget;
}

class ParamWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit ParamWidget(QWidget *parent = 0);
    ~ParamWidget();

protected slots:
    void onParaReadResult(Para _para);
    void onParaWriteResult(int result);
private slots:
    void on_btnSave_clicked();

private:
    Ui::ParamWidget *ui;
    ParaHandler *para;
    bool save_param();
    void loadLocalParam();

    // BaseWidget interface
public:
    virtual void hidenForGuest();
};

#endif // PARAMWIDGET_H
