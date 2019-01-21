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
    void EnableAlarm(bool en);
    void EnableZoom(bool en);
    void EnableClear(bool en);
    QString DisplayWeight(int weight, quint16 state, quint16 dot);
signals:
    void onDoubleClick(int addr,bool zoom);
    void onClearClick(int addr);

private:
    void SetOnline(bool online);
    Ui::DevWidget *ui;
    int m_addr;
    int m_timeout;
    bool m_en_alarm;
    int m_alarm_index;
    double m_alarm_value;
    bool m_last_still;
    bool m_online;
    bool m_zoom;
    bool m_en_zoom;
    QTime m_still_time;
    QString m_unit;
    QFont font;
    // QObject interface
    void clearState();
    void resetTimeout();

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void on_select(int addr);
private slots:
    void on_btnZero_triggered(QAction *arg1);
    void on_btnZero_clicked();

    // QWidget interface
};

#endif // DEVWIDGET_H
