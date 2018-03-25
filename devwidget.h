#ifndef DEVWIDGET_H
#define DEVWIDGET_H

#include <QWidget>

namespace Ui {
class DevWidget;
}

class DevWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DevWidget(int addr,QWidget *parent = 0);

    ~DevWidget();
    void Timeout();
    QString DisplayWeight(int weight, quint16 state, quint16 dot);
private:
    void SetOnline(bool online);
    Ui::DevWidget *ui;
    int m_addr;
    int m_timeout;

    // QObject interface
    void clearState();
    void resetTimeout();
};

#endif // DEVWIDGET_H
