#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <bitset>
#include "RtuReader.h"
#include "scanhandler.h"
#include "weighthandler.h"
#include "calibhandler.h"
#include "parahandler.h"
#include "adc102.h"
#include <QMap>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QApplication& app,QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onReadCalibParam(quint32 sensorMv, quint32 sensorFullSpan);
    void on_actionChagne_triggered();
    void calibrate_click(int id);
    void onParaReadResult(Para _para);
    void onScanResult(int type,int addr);
    void onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare);
    void onCalibProcessResult(int index, int result);
    void onReadCalibPointResult(int index, int weight, int ad);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void onRegOperResult(RegCmd cmd);
    void onWeightParaRead(quint16 div_high,quint16 div_low, quint32 full_high, quint32 full_low);
    void on_btnSearch_clicked();
    void onParaWriteResult(bool ok);
    void on_tabWidget_currentChanged(int index);

    void on_btnSave_clicked();

    void on_btnTare_clicked();

    void on_actionEnglish_triggered();

    void on_actionChinese_triggered();

    void on_btnAddr_clicked();

    void on_btnGN_clicked();


    void on_btnReset_clicked();

    void on_btnZero_clicked();

    void on_btnZoom10_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_btnSensorWrite_clicked();

private:
    void initCalibPoints();
    Ui::MainWindow *ui;

    QApplication &_app;
    ScanHandler *scaner;
    WeightHandler *weight;
    CalibHandler *calib;
    ParaHandler* para;
    RtuReader reader;
    QMap<QString,CmdHandler*> handlers;
    // QObject interface
    void traversalControl(const QObjectList &q);
    void clearCalib();
    void initUI();
    void changeHandler(QString name,bool start=true);
protected:
    void timerEvent(QTimerEvent *);

};

#endif // MAINWINDOW_H
