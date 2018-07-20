#ifndef UARTCFGWIDGET_H
#define UARTCFGWIDGET_H

#include <QWidget>

namespace Ui {
class UartCfgWidget;
}

class UartCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UartCfgWidget(QWidget *parent = 0);
    ~UartCfgWidget();

private:
    Ui::UartCfgWidget *ui;
};

#endif // UARTCFGWIDGET_H
