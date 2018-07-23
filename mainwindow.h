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
#include "QComSettingWidget.h"
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

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);



    void on_tabWidget_currentChanged(int index);


    void on_actionEnglish_triggered();

    void on_actionChinese_triggered();

    void on_btnAddr_clicked();



    void on_listWidget_itemClicked(QListWidgetItem *item);


	void on_btnNetConn_clicked();


    void on_actionReset_triggered();



    void on_comboBox_currentIndexChanged(int index);

private:
    void initCalibPoints();
    Ui::MainWindow *ui;

    QApplication &_app;
    ScanHandler *scaner;
    WeightHandler *weight;
    CalibHandler *calib;
    ParaHandler* para;
    CornHandler* corn;
    PollerHandler *poller;
    QList<QComSettingWidget*> comSettings;
    RtuReader reader;
    QMap<QString,CmdHandler*> handlers;
    MyConfig cfg;
    WaveWidget*  waveWidget;
    WaveWidget*  rtwaveWidget;
    DialogWave * waveDlg;
    int m_select_addr;
    QTime m_time;


    // QObject interface
    void traversalControl(const QObjectList &q);
    void clearCalib();
    void initUI();
    void changeHandler(QString name,bool start=true);

    void clearCornCalib(bool clear);
    void DisableAllCalibButton();
    void EnableAllCalibButton(bool disable);
    void EnableCalibButton(int index, bool disable);
    void endCornCalib();
    bool nextCalib(int index);
    bool finishEndCalib();
    void initAdList();


    QString formatValue(int value, int dot);
    void clear_cork_k();
    bool save_param();
    int toInt(QString txt, int dot,bool *ok);
    QString float2string(float wf, int dot);
    QString formatfloat(float wf, int dot);
    int ReadWaveList();
    void SetReadTimeout(int index, int count);
    void hideForGuest();

    void RegisterSlotPara();
protected:
    void timerEvent(QTimerEvent *);

protected slots:

    void on_actionChagne_triggered();
};

#endif // MAINWINDOW_H
