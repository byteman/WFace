#ifndef UARTCFGWIDGET_H
#define UARTCFGWIDGET_H

#include <QWidget>
#include "readerwidget.h"
namespace Ui {
class UartCfgWidget;
}

class UartSettingWidget : public IReaderWidget
{
    Q_OBJECT

public:
    explicit UartSettingWidget(QWidget *parent = 0);
    ~UartSettingWidget();

private:
    Ui::UartCfgWidget *ui;


};

#endif // UARTCFGWIDGET_H
