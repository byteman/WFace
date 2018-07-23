#include "UartSettingWidget.h"
#include "ui_UartSettingWidget.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
UartSettingWidget::UartSettingWidget(QWidget *parent) :
    IReaderWidget(parent),
    ui(new Ui::UartCfgWidget),
    scan(false)
{
    ui->setupUi(this);
    m_reader.start(100);
    QSerialPortInfo port;
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName());
    }
    ui->cbxBaud->setCurrentIndex(1);
    ui->scanPb->hide();
    connect(m_scaner,SIGNAL(scanResult(int,int)),this,SLOT(onScanResult(int,int)));

}

UartSettingWidget::~UartSettingWidget()
{
    delete ui;
}
void UartSettingWidget::onScanResult(int type,int addr)
{
    if(type == SCAN_FIND)
    {
        //QString title = QString("%1").arg(addr);
        //QListWidgetItem* item = new QListWidgetItem(QIcon(":/monitor.png"),title);
        //ui->listWidget->addItem(item);

        scan = true;
    }
    else if(type == SCAN_COMPLETE)
    {
        ui->btnSearch->setEnabled(true);
        ui->btnSearch->setText(tr("BusScan"));
        //ui->listWidget->setEnabled(true);
        scan = false;
        ui->scanPb->hide();
    }
    else if(type == SCAN_PROGRASS)
    {
        ui->scanPb->show();
        ui->scanPb->setValue(addr);
    }
}
void UartSettingWidget::on_btnSearch_clicked()
{
    if(!scan)
    {
        QString port = ui->cbxPort->currentText();//QString("COM%1").arg(ui->cbxPort->currentText());
        if(!m_reader.open(port,ui->cbxBaud->currentText().toInt(),'N',8,1))
        {
            QMessageBox::information(this,tr("error"),tr("uart open failed"));
            return ;
        }
        m_scaner->init(3,1,1,33,!ui->cbxFindAll->isChecked());
        m_scaner->start();
        ui->btnSearch->setText(tr("StopSearch"));
//        ui->listWidget->setEnabled(false);
//        ui->listWidget->clear();
//        ui->listWidget->setIconSize(QSize(64,64));
        emit StartSearch(this,true);
        scan = true;
    }
    else
    {
        m_scaner->stop();
        emit StartSearch(this,false);
    }
}
