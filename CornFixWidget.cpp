#include "CornFixWidget.h"
#include "ui_CornFixWidget.h"
#include <QSignalMapper>
CornFixWidget::CornFixWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::CornFixWidget)
{
    ui->setupUi(this);


    connect(corn,SIGNAL(chanADReadResult(QList<float>)),this,SLOT(chanADReadResult(QList<float>)));
    connect(corn,SIGNAL(chanKReadResult(int,QList<float>)),this,SLOT(chanKReadResult(int,QList<float>)));
    connect(corn,SIGNAL(OperationResult(RegCmd)),this,SLOT(onRegOperResult(RegCmd)));


}
//继续下一步标定
bool CornFixWidget::nextCalib(int index)
{

    EnableAllCalibButton(false);
    ui->btnStopCalib->setEnabled(false);
    if(corn->getSensorNum() > index)
    {
        //启动成功后，有大于1个传感器,就启动第2个传感器.
        EnableCalibButton(index,true);
        QMessageBox::information(this,tr("corn_calib_title"),tr("corn_calib_msg") + tr("click_next"));
        return true;
    }
    else
    {
        //否则结束了,让停止键可用.
        QMessageBox::information(this,tr("corn_calib_title"),tr("corn_calib_msg") + tr("click_stop"));
        ui->btnStopCalib->setEnabled(true);
        return false;
    }
}
void CornFixWidget::EnableAllCalibButton(bool disable)
{
    int rows = ui->tblCornFix->rowCount();

    for(int i = 0; i < rows; i++)
    {
        QPushButton* btn = (QPushButton*)ui->tblCornFix->cellWidget(i,2);
        if(btn!=NULL)
        {
            btn->setEnabled(disable);
        }

    }
}
void CornFixWidget::EnableCalibButton(int index,bool disable)
{
    QPushButton* btn = (QPushButton*)ui->tblCornFix->cellWidget(index,2);
    if(btn!=NULL)
    {
        btn->setEnabled(disable);
    }
}
void CornFixWidget::initCornFixChan()
{
    //ui->tblCornFix->verticalHeader()->setHidden(true);
    ui->tblCornFix->setRowCount(4);
    ui->tblCornFix->setColumnCount(3);
    QStringList col_headers;
    //col_headers.push_back(tr("channel"));
    col_headers.push_back(tr("ad"));
    col_headers.push_back(tr("k"));
    col_headers.push_back(tr("Operation"));


    ui->tblCornFix->setHorizontalHeaderLabels(col_headers);

    QStringList row_headers;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i <=  3; i++)
    {
        QString title = QString("%1%2%3").arg(tr("calib")).arg(tr("corn")).arg(i+1);
        QPushButton* button = new QPushButton(title,ui->tblCornFix);
        button->setGeometry(0,0,80,50);
        button->setEnabled(false);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, i);

        ui->tblCornFix->setCellWidget(i,2,button);
        for(int j = 0 ; j < 2; j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tblCornFix->setItem(i,j,item);
        }
        row_headers.push_back(QString(tr("channel")) + QString("%1").arg(i+1));

    }

    ui->tblCornFix->setVerticalHeaderLabels(row_headers);
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(corn_calibrate_click(int)));
    ui->btnStopCalib->setEnabled(false);

}
void CornFixWidget::clearCornCalib(bool clear)
{
    int rows = ui->tblCornFix->rowCount();
    int cols = ui->tblCornFix->columnCount();
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QTableWidgetItem* item = ui->tblCornFix->item(i,j);
            if(item!=NULL)
                item->setText("");
        }
    }
    if(clear)
        ui->edtSensorNum->setText("");

}
void CornFixWidget::corn_calibrate_click(int id)
{
    qDebug() << id << "---clicked";
    corn->calib(id+1);
//    int weight = ui->tblCornFix->item(id,1)->text().toInt();
//    calib->calibSet(id,weight,0);
}


void CornFixWidget::onRegOperResult(RegCmd cmd)
{
    if(!cmd.isRead)
    {
        if( (cmd.reg_addr == REG_FULL_SPAN)||
                (cmd.reg_addr == REG_4B_CORN_K))
        {
            if(cmd.error != REG_ERROR_OK)
            {
                QMessageBox::information(this,tr("error"),tr("save_fail"));
            }
            else
            {

                if((cmd.reg_addr == REG_2B_SENSOR_NUM) ||
                        (cmd.reg_addr == REG_4B_CORN_K))
                {
                    //写完参数后，重新读取一下参数
                    corn->ReadParam();
                }

                QMessageBox::information(this,tr("info"),tr("save_ok"));


            }
        }

    }


}
//K系数读取结果.
void CornFixWidget::chanKReadResult(int sensor, QList<float> chanK)
{

    for(int i = 0; i < chanK.size();i++)
    {
        QTableWidgetItem* item = ui->tblCornFix->item(i,1);
        if(item!=NULL)
        {
            item->setText(QString("%1").arg(chanK[i]));
        }

    }
    ui->edtSensorNum->setText(QString("%1").arg(sensor));
}
void CornFixWidget::clear_cork_k()
{
    int rows = ui->tblCornFix->rowCount();

    for(int i = 0; i < rows; i++)
    {

        QTableWidgetItem* item = ui->tblCornFix->item(i,1);
        if(item!=NULL)
            item->setText("");

    }
    ui->edtSensorNum->setText("");
}
//角差标定界面显示的实时AD
void CornFixWidget::chanADReadResult(QList<float> chanAD)
{
    QTableWidgetItem *item = NULL;

    for(int i = 0; i < chanAD.size();i++)
    {
        item = ui->tblCornFix->item(i,0);
        if(item!=NULL)
            item->setText(QString("%1").arg(chanAD[i]));
    }

}
CornFixWidget::~CornFixWidget()
{
    delete ui;
}

void CornFixWidget::on_btnReadSrs_clicked()
{
    clear_cork_k();
    corn->ReadParam();
}

void CornFixWidget::on_btnSrsWrite_clicked()
{
    bool ok = false;
    int num = ui->edtSensorNum->text().toInt(&ok);

    if(num > 4 || num < 1)
    {
        return;
    }
    corn->setSensorNum(num);

    clearCornCalib(false);
     corn->ReadParam();

#if 0
    QList<float> ks;
    int rows = ui->tblCornFix->rowCount();

    num = 0;
    for(int i = 0; i < rows; i++)
    {

        QTableWidgetItem* item = ui->tblCornFix->item(i,1);
        if(item!=NULL)
        {
            bool ok = false;
            QString sk = item->text();
            float k = sk.toFloat(&ok);
            if(!ok)
            {
                continue;
                //QString err= item->text()+QString(tr(" format_err"));
                //QMessageBox::information(this,tr("error"),err);
                //return;
            }
            ks.push_back(k);
            num++;
        }
    }

    corn->setKs(ks);
#endif
}
bool CornFixWidget::finishEndCalib()
{
    EnableAllCalibButton(false);
    ui->btnStopCalib->setEnabled(false);
    ui->btnStartCalib->setEnabled(true);
    corn->ReadParam();
    QMessageBox::information(this,tr("corn_calib_title"),tr("stop_calib_msg"));
    return true;
}

void CornFixWidget::on_btnStartCalib_clicked()
{
     corn->startCalib();
}

void CornFixWidget::on_btnStopCalib_clicked()
{
    corn->stopCalib();
}
