#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adc102.h"
#include <QListWidgetItem>
#include <bitset>
#include <QSignalMapper>
#include <QTableWidgetItem>
#include <QTcpServer>
#include <QTcpSocket>
#include "networkmgr.h"
#include "customparams.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QApplication& app,QWidget *parent = 0);
    ~MainWindow();

    void addItemContent(int row, int column, QString content);

private slots:
    void onTimerHandle();
    void onNewClient(NetClient*);
    void onOneMsg(NetClient*,Msg_Head,void*);
    void on_actionChagne_triggered();
    void calibrate_click(int id);
    void calibrate_click_zero(int id);
    void onParaReadResult(Para _para);
    void onScanResult(int type,int addr);
    void onUpdateResult(int result, int pos, int total);
    void onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare);
    void onCalibProcessResult(int index, int result);
    void onReadCalibPointResult(Sensor *sensors, int num,int weight,float k);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void onUpdateEvent(NetClient* _client,int evt, UpdateEvtPara para);
    void on_btnSearch_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_btnSave_clicked();

    void on_btnTare_clicked();

    void on_actionEnglish_triggered();

    void on_actionChinese_triggered();

    void on_btnAddr_clicked();

    void on_btnGN_clicked();

    void on_btnUpdate_clicked();

    void on_btnSelFile_clicked();

    void on_btnReset_clicked();

    void on_btnZero_clicked();

    void on_btnZoom10_clicked();

    void on_btnCalibAllZero_clicked();

    void on_btnCalibAllWt_clicked();
    void onRemoveClient(int);


    void on_btnModifyK_clicked();

    void on_btnCalibK_clicked();

    void on_radioAudo_clicked();

    void on_radioHand_clicked();

    void on_edtVersion_returnPressed();

    void on_edtDevTime_returnPressed();

    void on_edtGpsTime_returnPressed();

    void on_cbxGps_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_btnQuitCalib_clicked();

private:
    void initCalibPoints(int count);
    Ui::MainWindow *ui;
    ADC102 adc102;
    QApplication &_app;
    QTimer m_timer;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;
    NetWorkMgr network;
    QString curDev;
    bool closed;
    bool isUart;
    QList<int> m_read_cmds;
    CustomParamsManager cpm;
    // QObject interface
public:

    void parse();
    void processOneWeight(QByteArray &data);
    void addItem(QString id);

    // QWidget interface
    QUrl GPSMark(const QString &GPSCoordinate);
    bool checkAck(QString para, int oper,int value);
    void removeCmd(int cmd);
    void traversalControl(const QObjectList &q);
protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
