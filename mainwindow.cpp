#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include <QMessageBox>
#include <qdebug.h>
#include "3rdparty/qextserialport/qextserialenumerator.h"
#include <QSignalMapper>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file(":/mystyle.txt");
    file.open(QFile::ReadOnly);


    QByteArray res = file.readAll();


    QextSerialEnumerator serialEnum;
    QList<QextPortInfo> ports = serialEnum.getPorts();
    QextPortInfo port;
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName);
    }
    ui->cbxBaud->setCurrentIndex(1);
    //int version = 21101;
    //ui->edtVersion->setText(QString("V%1.%2.%3").arg(version/10000).arg((version%10000)/100).arg(version%100));
    connect(&adc102,SIGNAL(scanResult(int , int )),this,SLOT(onScanResult(int,int)));
    connect(&adc102,SIGNAL(weightResult(int , quint16 )),this,SLOT(onWeightResult(int,quint16)));
    connect(&adc102,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    connect(ui->btnTare,SIGNAL(clicked(bool)),&adc102,SLOT(discardTare()));
    connect(ui->btnZero,SIGNAL(clicked(bool)),&adc102,SLOT(setZero()));
    connect(ui->btnZoom10,SIGNAL(clicked(bool)),&adc102,SLOT(zoom10X()));
    initCalibPoints();
    //connect(ui->btnGN,SIGNAL(clicked(bool)),&adc102,SLOT(discardTare()));
    //this->setStyleSheet(res);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionChagne_triggered()
{
    QMessageBox::about(this,"title","click write parameter");
}

void MainWindow::calibrate_click(int id)
{
    qDebug() << id << "---clicked";
}

void MainWindow::onParaReadResult(Para _para)
{
    ui->cbxDot->setCurrentIndex(_para.dot);
    ui->edtFullLow->setText(QString("%1").arg(_para.span_low));
    ui->edtFullHigh->setText(QString("%1").arg(_para.span_high));
    ui->cbxDivHigh->setCurrentText(QString("%1").arg(_para.div_high));
    ui->cbxDivLow->setCurrentText(QString("%1").arg(_para.div_low));
    ui->cbxUnit->setCurrentIndex(_para.unit);
    ui->edtZeroSpan->setText(QString("%1").arg(_para.zero_track_span));
    ui->edtStableSpan->setText(QString("%1").arg(_para.stable_span));
    ui->edtHandZeroSpan->setText(QString("%1").arg(_para.hand_zero_span));
    ui->edtSensorFullSpan->setText(QString("%1").arg(_para.sensor_full_span));
    ui->edtSensorMv->setText(QString("%1").arg(_para.sensor_mv));
    ui->edtSlaveAddr->setText(QString("%1").arg(_para.slave_addr));
    ui->edtPwrZeroSpan->setText(QString("%1").arg(_para.pwr_zero_span));
    ui->cbxFilterLvl->setCurrentIndex(_para.filter_level);
    ui->edtVersion->setText(QString("V%1.%2.%3").arg(_para.version/10000).arg((_para.version%10000)/100).arg(_para.version%100));
}



void MainWindow::onScanResult(int type,int addr)
{
    if(type == 0)
    {
        QString title = QString("%1").arg(addr);
        QListWidgetItem* item = new QListWidgetItem(QIcon(":/monitor.png"),title);
        ui->listWidget->addItem(item);
    }
    else
    {
        ui->btnSearch->setEnabled(true);
        ui->listWidget->setEnabled(true);
    }
}

void MainWindow::onWeightResult(int weight, quint16 state)
{
    ui->lbl_display_wet->setText(QString("%1").arg(weight));

}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    adc102.setSlaveAddr(item->text().toInt());
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_btnSearch_clicked()
{
    QString port = ui->cbxPort->currentText();//QString("COM%1").arg(ui->cbxPort->currentText());
    if(!adc102.startScan(port,ui->cbxBaud->currentText().toInt(),'N',8,1))
    {
        QMessageBox::information(this,tr("错误"),tr("串口打开失败"));
        return ;
    }
    ui->btnSearch->setEnabled(false);
    ui->listWidget->setEnabled(false);
    ui->listWidget->clear();
    ui->listWidget->setIconSize(QSize(64,64));
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(!adc102.hasConnect())
    {
        if(index != 0)
        {
            ui->tabWidget->setCurrentIndex(0);
        }
        QMessageBox::information(this,tr("info"),tr("please scan device first"));
        return ;
    }

    if(index == 0)
    {

    }
    else if(index == 1)
    {
        adc102.startReadWeight();
    }
    else if(index == 2)
    {
        adc102.startReadPara();
    }
}

void MainWindow::on_btnSave_clicked()
{
    Para p;
    p.filter_level = ui->cbxFilterLvl->currentIndex();
    p.div_high = ui->cbxDivHigh->currentText().toInt();
    p.div_low = ui->cbxDivLow->currentText().toInt();
    p.dot = ui->cbxDot->currentIndex();
    p.hand_zero_span = ui->edtHandZeroSpan->text().toInt();
    p.pwr_zero_span  = ui->edtPwrZeroSpan->text().toInt();
    p.sensor_full_span = ui->edtSensorFullSpan->text().toInt();
    p.sensor_mv = ui->edtSensorMv->text().toInt();
    p.slave_addr = ui->edtSlaveAddr->text().toInt();
    p.span_high = ui->edtFullHigh->text().toInt();
    p.span_low = ui->edtFullLow->text().toInt();
    p.stable_span = ui->edtStableSpan->text().toInt();
    p.unit = ui->cbxUnit->currentIndex();
    p.zero_track_span = ui->edtZeroSpan->text().toInt();
    adc102.paraSave(p);
}

void MainWindow::on_btnTare_clicked()
{

}

void MainWindow::initCalibPoints()
{

    ui->tblCalib->setRowCount(6);
    ui->tblCalib->setColumnCount(3);
    QStringList col_headers;
    col_headers.push_back(tr("ad"));
    col_headers.push_back(tr("weight"));
    col_headers.push_back(tr("calibrate"));

    ui->tblCalib->setHorizontalHeaderLabels(col_headers);

    QStringList row_headers;
    //row_headers<<"0" <<"1" << "2" << "3" << "4" << "5";

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i <=  5; i++)
    {
        QPushButton* button = new QPushButton(tr("calib"),ui->tblCalib);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, i);
        ui->tblCalib->setCellWidget(i,2,button);
        row_headers.push_back(QString("%1").arg(i));

    }
    ui->tblCalib->setVerticalHeaderLabels(row_headers);
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(calibrate_click(int)));
}
