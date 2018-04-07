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
#include "dialogwave.h"
#include "myconfig.h"
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

    void on_actionChagne_triggered();

    void onParaReadResult(Para _para);
    void onScanResult(int type,int addr);
    void onPollTimeout(int addr);
    void onPollWeightResult(int addr,int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);


    void on_btnSearch_clicked();
    void onParaWriteResult(int result);
    void on_tabWidget_currentChanged(int index);



    void on_btnAddr_clicked();





    void on_listWidget_itemClicked(QListWidgetItem *item);


    void on_btnSaveWave_clicked();
    void on_btnSetAddr_clicked();
    void on_listWave_itemClicked(QListWidgetItem *item);
    void on_btnClearWave_clicked();
    void on_btnClear_clicked();
    void on_edtSaveTime_valueChanged(int arg1);
    void on_cbxAcqSpan_currentIndexChanged(int index);
    void on_btnZero_clicked();

    void on_btnRead_clicked();

    void on_btnCalibZero_clicked();

    void on_btnSave_clicked();

    void on_btnCalibWgt_clicked();

    void on_btnQueryError_clicked();

    void on_btnQueryChange_clicked();

    void on_btnModifyAddr_clicked();

    void on_btnChange_clicked();

private:

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
    WaveWidget*  waveWidget;
    WaveWidget*  rtwaveWidget;
    DialogWave * waveDlg;
    int m_select_addr;
    QTime m_time;
    MyConfig cfg;
    bool pressed;
    // QObject interface
    void traversalControl(const QObjectList &q);
    void clearCalib();
    void initUI();
    void changeHandler(QString name,bool start=true);


    QString formatValue(int value, int dot);
    void clear_cork_k();
    bool save_param();
    int toInt(QString txt, int dot,bool *ok);
    QString float2string(float wf, int dot);
    QString formatfloat(float wf, int dot);
    int ReadWaveList();
    void SetReadTimeout(int index, int count);

    QString formatState(SensorState &state);
protected:
    void timerEvent(QTimerEvent *);

};

#endif // MAINWINDOW_H
