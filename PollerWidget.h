#ifndef POLLERWIDGET_H
#define POLLERWIDGET_H

#include <QWidget>
#include "pollhandler.h"
#include "mydevices.h"
#include "wavewidget.h"
#include "BaseWidget.h"
namespace Ui {
class PollerWidget;
}

class PollerWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit PollerWidget(QWidget *parent = 0);
    ~PollerWidget();

protected slots:
    void onPollWeightResult(int addr, int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare);
    void onPollTimeout(int addr);
private slots:
    void on_btnSetAddr_clicked();

    void on_btnSaveWave_clicked();

    void on_btnClear_clicked();

    void on_cbxAcqSpan_currentIndexChanged(int index);

    void on_edtSaveTime_valueChanged(int arg1);

private:
    Ui::PollerWidget *ui;
    PollerHandler *poller;

    WaveWidget*  rtwaveWidget;
    MyDevices* devices;
    QTime m_time;
    void SetReadTimeout(int index, int count);
};

#endif // POLLERWIDGET_H
