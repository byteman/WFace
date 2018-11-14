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
    void Show();
    void Reset();
    QString DisplayWeight(int weight, quint16 state, quint16 dot);
signals:
    void onDoubleClick(int addr,bool zoom);
private:
    void SetOnline(bool online);
    Ui::DevWidget *ui;
    int m_addr;
    int m_timeout;
    int m_alarm_index;
    double m_alarm_value;
    bool m_last_still;
    bool m_online;
    bool m_zoom;
    QTime m_still_time;
    QString m_unit;
    QFont font;
    // QObject interface
    void clearState();
    void resetTimeout();

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // DEVWIDGET_H
