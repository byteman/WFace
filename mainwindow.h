#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adc102.h"
#include <QListWidgetItem>
#include <bitset>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionChagne_triggered();
    void calibrate_click(int id);
    void onParaReadResult(Para _para);
    void onScanResult(int type,int addr);
    void onWeightResult(int weight, quint16 state);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_btnSearch_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_btnSave_clicked();

    void on_btnTare_clicked();

private:
    void initCalibPoints();
    Ui::MainWindow *ui;
    ADC102 adc102;

};

#endif // MAINWINDOW_H
