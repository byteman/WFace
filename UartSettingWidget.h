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

protected slots:
    void onScanResult(int type, int addr);
private slots:
    void on_btnSearch_clicked();

private:
    Ui::UartCfgWidget *ui;
    bool scan;


};

#endif // UARTCFGWIDGET_H
