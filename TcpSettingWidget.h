#ifndef TCPSETTINGWIDGET_H
#define TCPSETTINGWIDGET_H

#include <QWidget>
#include "readerwidget.h"
namespace Ui {
class TcpSettingWidget;
}

class TcpSettingWidget : public IReaderWidget
{
    Q_OBJECT

public:
    explicit TcpSettingWidget(QWidget *parent = 0);
    ~TcpSettingWidget();

private:
    Ui::TcpSettingWidget *ui;
};

#endif // TCPSETTINGWIDGET_H
