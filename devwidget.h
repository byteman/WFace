#ifndef DEVWIDGET_H
#define DEVWIDGET_H

#include <QWidget>
#include <QTime>
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
    void SetUnit(QString unit);
    void SetAlarmSetting(int setting, double value);
    QString DisplayWeight(int weight, quint16 state, quint16 dot);
private:
    void SetOnline(bool online);
    Ui::DevWidget *ui;
    int m_addr;
    int m_timeout;
    int m_alarm_index;
    double m_alarm_value;
    bool m_last_still;
    bool m_online;
    QTime m_still_time;
    QString m_unit;
    // QObject interface
    void clearState();
    void resetTimeout();
};

#endif // DEVWIDGET_H
