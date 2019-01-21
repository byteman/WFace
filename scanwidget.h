#ifndef SCAN_WIDGET_H
#define SCAN_WIDGET_H

#include <QWidget>
#include <QTime>
#include <QMenu>
namespace Ui {
class ScanWidget;
}

class ScanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScanWidget(int addr,QWidget *parent = 0);

    ~ScanWidget();
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
    void onModifyDevAddr(int oldAddr,int newAddr);
private:
    void SetOnline(bool online);
    Ui::ScanWidget *ui;
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
    QMenu menu;
    // QObject interface
    void clearState();
    void resetTimeout();

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
private slots:
    void on_btnZero_triggered(QAction *arg1);
    void on_btnZero_clicked();
    void OnModifyAddr();
    // QWidget interface

protected slots:
    void on_lbl_addr_customContextMenuRequested(const QPoint &pos);
    void on_ScanWidget_customContextMenuRequested(const QPoint &pos);
};

#endif // ScanWidget_H
