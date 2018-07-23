#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <QWidget>
#include <QLabel>
#include "calibhandler.h"
#include "BaseWidget.h"
namespace Ui {
class CalibWidget;
}

class CalibWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit CalibWidget(QWidget *parent = 0);
    ~CalibWidget();

protected slots:
    void onRegOperResult(RegCmd cmd);
    void onReadCalibPointResult(int index, int weight, int ad, int dot);
    void onCalibProcessResult(int index, int result);
    void calibrate_click(int id);
    void calibADReadResult(QList<float> chanAD);
private slots:
    void on_tblCalib_cellChanged(int row, int column);

    void on_tblCalib_cellPressed(int row, int column);

private:
    Ui::CalibWidget *ui;
    CalibHandler *calib;
    QList<QLabel*> adlist;
    void initAdList();
    bool nextCalib(int index);
    bool finishEndCalib();
    void clearCalib();
    bool pressed;
    void EnableAllCalibButton(bool disable);
    void EnableCalibButton(int index, bool disable);
    void initCalibPoints();
};

#endif // CALIBWIDGET_H
