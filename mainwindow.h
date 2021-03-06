#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adc102.h"
#include <QListWidgetItem>
#include <bitset>
#include "RtuReader.h"
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
    void calibrate_click(int id);
    void onParaReadResult(Para _para);
    void onScanResult(int type,int addr);
    void onUpdateResult(int result, int pos, int total);
    void onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare);
    void onCalibProcessResult(int index, int result);
    void onReadCalibPointResult(int index, int weight, int ad);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_btnSearch_clicked();

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

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_btnSensorWrite_clicked();

private:
    void initCalibPoints();
    Ui::MainWindow *ui;
    ADC102 adc102;
    QApplication &_app;

    RtuReader reader;
    // QObject interface
    void traversalControl(const QObjectList &q);
    void clearCalib();
protected:
    void timerEvent(QTimerEvent *);

};

#endif // MAINWINDOW_H
