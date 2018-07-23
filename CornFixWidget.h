#ifndef CORNFIXWIDGET_H
#define CORNFIXWIDGET_H

#include <QWidget>
#include "cornhandler.h"
#include "BaseWidget.h"
namespace Ui {
class CornFixWidget;
}

class CornFixWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit CornFixWidget(QWidget *parent = 0);
    ~CornFixWidget();

protected slots:
    void chanADReadResult(QList<float> chanAD);
    void chanKReadResult(int sensor, QList<float> chanK);
    void corn_calibrate_click(int id);
    void onRegOperResult(RegCmd cmd);
private slots:
    void on_btnReadSrs_clicked();

    void on_btnSrsWrite_clicked();

    void on_btnStartCalib_clicked();

    void on_btnStopCalib_clicked();

private:
    Ui::CornFixWidget *ui;
    CornHandler *corn;
    void clearCornCalib(bool clear);
    void clear_cork_k();
    void initCornFixChan();
    bool nextCalib(int index);
    void EnableAllCalibButton(bool disable);
    void EnableCalibButton(int index, bool disable);
    bool finishEndCalib();
};

#endif // CORNFIXWIDGET_H
