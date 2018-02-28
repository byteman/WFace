#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QLabel>
#include <bitset>
#include "RtuReader.h"
#include "scanhandler.h"
#include "weighthandler.h"
#include "calibhandler.h"
#include "parahandler.h"
#include "cornhandler.h"
#include "pollhandler.h"
#include "adc102.h"
#include "mydevices.h"
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
    void onPollWeightResult(int addr,int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare);
    void onCalibProcessResult(int index, int result);
    void onReadCalibPointResult(int index, int weight, int ad ,int dot);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void onRegOperResult(RegCmd cmd);
    void onWeightParaRead(quint16 div_high,quint16 div_low, quint32 full_high, quint32 full_low, int dot);
    void on_btnSearch_clicked();
    void onParaWriteResult(bool ok);
    void on_tabWidget_currentChanged(int index);
    void chanADReadResult(QList<float> chanAD);
    void calibADReadResult(QList<float> chanAD);
    void chanKReadResult(int sensor, QList<float> chanK);
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

    void on_btnStartCalib_clicked();
    void on_btnReadK_clicked();

    void on_btnReadSrs_clicked();

    void on_btnSrsWrite_clicked();

    void on_btnWriteK_clicked();

    void on_btnStopCalib_clicked();

    void on_tblCalib_cellEntered(int row, int column);

    void on_tblCalib_cellChanged(int row, int column);

    void on_tblCalib_cellPressed(int row, int column);

    void on_btnSetAddr_clicked();

    void on_btnOpen_clicked();

private:
    void initCalibPoints();
    Ui::MainWindow *ui;
    QList<QLabel*> adlist;
    QApplication &_app;
    ScanHandler *scaner;
    WeightHandler *weight;
    CalibHandler *calib;
    ParaHandler* para;
    CornHandler* corn;
    PollerHandler *poller;
    RtuReader reader;
    QMap<QString,CmdHandler*> handlers;
    MyDevices* devices;
    bool pressed;
    // QObject interface
    void traversalControl(const QObjectList &q);
    void clearCalib();
    void initUI();
    void changeHandler(QString name,bool start=true);
    void initCornFixChan();
    void clearCornCalib();
    void DisableAllCalibButton();
    void EnableAllCalibButton(bool disable);
    void EnableCalibButton(int index, bool disable);
    void endCornCalib();
    bool nextCalib(int index);
    bool finishEndCalib();
    void initAdList();
    void clearState();

    QString formatValue(int value, int dot);
    void clear_cork_k();
    bool save_param();
    int toInt(QString txt, int dot,bool *ok);
    QString float2string(float wf, int dot);
    QString formatfloat(float wf, int dot);
protected:
    void timerEvent(QTimerEvent *);

protected slots:
    void corn_calibrate_click(int id);
};

#endif // MAINWINDOW_H
