#include "CalibWidget.h"
#include "ui_CalibWidget.h"
#include <QMessageBox>
#include <QSignalMapper>
#include <QPushButton>
#include <QDebug>
CalibWidget::CalibWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::CalibWidget)
{
    ui->setupUi(this);

    initAdList();
    initCalibPoints();
    connect(calib,SIGNAL(calibReadResult(int,qint32,qint32,int)),this,SLOT(onReadCalibPointResult(int,int,int,int)));
    connect(calib,SIGNAL(calibParaResult(quint32,quint32)),this,SLOT(onReadCalibParam(quint32,quint32)));
    connect(calib,SIGNAL(OperationResult(RegCmd)),this,SLOT(onRegOperResult(RegCmd)));
    connect(calib,SIGNAL(chanADReadResult(QList<float>)),this,SLOT(calibADReadResult(QList<float>)));

}
void CalibWidget::initCalibPoints()
{
    //ui->tblCalib->verticalHeader()->setHidden(true);
    ui->tblCalib->setRowCount(6);
    ui->tblCalib->setColumnCount(3);
    QStringList col_headers;
    col_headers.push_back(tr("ad"));
    col_headers.push_back(tr("weight"));
    col_headers.push_back(tr("calibrate"));

    ui->tblCalib->setHorizontalHeaderLabels(col_headers);

    QStringList row_headers;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i <=  5; i++)
    {
        QString title;
        if(i == 0) title = tr("calib_zero");
        else title = QString("%1%2%3").arg(tr("calib")).arg(i).arg(tr("span"));
        QPushButton* button = new QPushButton(title,ui->tblCalib);
        button->setGeometry(0,0,80,50);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, i);
        ui->tblCalib->setCellWidget(i,2,button);
        for(int j = 0 ; j < 2; j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tblCalib->setItem(i,j,item);
        }
        row_headers.push_back(QString("%1").arg(i+1));

    }
    ui->tblCalib->setVerticalHeaderLabels(row_headers);
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(calibrate_click(int)));
}
void CalibWidget::calibrate_click(int id)
{
    qDebug() << id << "---clicked";
    bool ok = false;
    float weight = ui->tblCalib->item(id,1)->text().toFloat(&ok);
    if(!ok)
    {
        QMessageBox::information(this,tr("error"),tr("format_err"));
        return;
    }
    if(!calib->calibSet(id,weight,0))
    {
        QMessageBox::information(this,tr("error"),tr("calib_fail"));
    }
}
void CalibWidget::initAdList()
{
    adlist.push_back(ui->lblAD1_2);
    adlist.push_back(ui->lblAD2_2);
    adlist.push_back(ui->lblAD3_2);
    adlist.push_back(ui->lblAD4_2);
    for(int i = 0; i < adlist.size();i++)
    {
        adlist[i]->clear();
    }
}
//标定界面显示的实时AD
void CalibWidget::calibADReadResult(QList<float> chanAD)
{

    for(int i = 0; i < adlist.size(); i++)
    {
        adlist[i]->clear();
    }
    for(int i = 0; i < chanAD.size();i++)
    {
        QString text = QString("AD%1:%2").arg(i+1).arg(chanAD[i]);
        adlist[i]->setText(text);
    }
}
//标定过程....
void CalibWidget::onCalibProcessResult(int index, int result)
{
    if(result > 0)
    {
        //ui->statusBar->showMessage(QString("left time %1S").arg(result));
    }
    else if(result == 0)
    {
        QMessageBox::information(this,tr("info"),tr("calib ok"));
    }
    else
    {
        QMessageBox::information(this,tr("info"),tr("calib failed"));
    }
}

void CalibWidget::onReadCalibPointResult(int index, int weight, int ad ,int dot)
{

    QTableWidgetItem *itemAd = ui->tblCalib->item(index,0);
    if(itemAd!=NULL)
    {
        itemAd->setText(QString("%1").arg(ad));
    }
    QTableWidgetItem *itemWt = ui->tblCalib->item(index,1);
    if(itemWt!=NULL)
    {
        itemWt->setText(QString("%1").arg(utils::formatValue(weight,dot)));
    }

}
void CalibWidget::onRegOperResult(RegCmd cmd)
{
    qDebug() << "onRegOperResult " << "reg=" << cmd.reg_addr << " num=" << cmd.reg_num << " " << cmd.isRead;
    if(cmd.reg_addr == REG_CALIB)
    {
        //标定完成后，重新读取标定后的值.
        calib->readPara();
        if(cmd.error == REG_ERROR_OK)
        {
            QMessageBox::information(this,tr("info"),tr("calib_ok"));
            return ;
        }
        else
        {
            QMessageBox::information(this,tr("error"),tr("calib_fail"));
            return ;
        }
    }
    else if(!cmd.isRead)
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
                    //corn->ReadParam();
                }

                QMessageBox::information(this,tr("info"),tr("save_ok"));


            }
        }


    }


}



void CalibWidget::clearCalib()
{
    int rows = ui->tblCalib->rowCount();
    int cols = ui->tblCalib->columnCount();
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QTableWidgetItem* item = ui->tblCalib->item(i,j);
            if(item!=NULL)
                item->setText("");
        }
    }
}


CalibWidget::~CalibWidget()
{
    delete ui;
}

void CalibWidget::on_tblCalib_cellChanged(int row, int column)
{
    if(pressed)
    {
        qDebug() << "changed" << row << column;
        pressed = false;
        QTableWidgetItem* item = ui->tblCalib->item(row,column);
        if(item!=NULL)
        {
            bool ok = false;
            float v = item->text().toFloat(&ok);
            item->setText(utils::formatfloat(v,calib->getDot()));
        }
    }
}

void CalibWidget::on_tblCalib_cellPressed(int row, int column)
{
    pressed = true;
}
