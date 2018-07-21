#include "QComSettingWidget.h"
#include "ui_QComSettingWidget.h"
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDebug>

QComSettingWidget::QComSettingWidget(QWidget *parent,QString pname) :
    IReaderWidget(parent),
    m_port(pname),
    ui(new Ui::QComSettingWidget)
{
    ui->setupUi(this);
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QSerialPortInfo port;
    bool find = false;
    m_port = QString("COM%1").arg(pname);
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName());
        if(port.portName() == m_port){
            find =true;
        }
    }
    if(find){
        ui->cbxPort->setCurrentText(m_port);
    }

    ui->cbxBaud->setCurrentIndex(1);
    //ui->lblState->setText("");
    ui->btnComOpera->setText("Open");
}

QComSettingWidget::~QComSettingWidget()
{
    delete ui;
}


void QComSettingWidget::on_btnComOpera_clicked()
{
    qDebug() << "on_btnComOpera_clicked";
    //if(m_reader.hasConnected()){
        QString port = ui->cbxPort->currentText();
        if(!m_reader.open(port,ui->cbxBaud->currentText().toInt(),'N',8,1))
        {
            //ui->lblState->setText("failed");
            QMessageBox::information(this,tr("error"),tr("uart open failed"));

            return ;
        }else{
            //ui->lblState->setText("open");
            ui->btnComOpera->setText("Close");
            QMessageBox::information(this,tr("info"),tr("uart open success"));
            m_reader.setDeviceAddr(1);
        }
    //}
}

