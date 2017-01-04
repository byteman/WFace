#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include <QMessageBox>
#include <qdebug.h>
#include "3rdparty/qextserialport/qextserialenumerator.h"
#include <QSignalMapper>
#include <QTranslator>
#include "pcomm.h"
#include <QFileDialog>
#include <QFile>
#include "netmsg.h"
#include "crc.h"
static QString unit="g";
MainWindow::MainWindow(QApplication &app,QWidget *parent) :
    QMainWindow(parent),
    _app(app),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
qRegisterMetaType<Para>("Para");
    QFile file(":/mystyle.txt");
    file.open(QFile::ReadOnly);


    QByteArray res = file.readAll();


    QextSerialEnumerator serialEnum;
    QList<QextPortInfo> ports = serialEnum.getPorts();
    QextPortInfo port;
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName);
    }
    ui->cbxBaud->setCurrentIndex(4);
    ui->progressBar->hide();
    //int version = 21101;
    //ui->edtVersion->setText(QString("V%1.%2.%3").arg(version/10000).arg((version%10000)/100).arg(version%100));
    connect(&adc102,SIGNAL(scanResult(int , int )),this,SLOT(onScanResult(int,int)));
    connect(&adc102,SIGNAL(weightResult(int , quint16,quint16, qint32,qint32 )),this,SLOT(onWeightResult(int,quint16,quint16, qint32,qint32)));
    connect(&adc102,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    //connect(ui->btnTare,SIGNAL(clicked(bool)),&adc102,SLOT(discardTare()));
    //connect(ui->btnZero,SIGNAL(clicked(bool)),&adc102,SLOT(setZero()));
    //connect(ui->btnZoom10,SIGNAL(clicked(bool)),&adc102,SLOT(zoom10X()));
    connect(&adc102,SIGNAL(calibProcessResult(int,int)),SLOT(onCalibProcessResult(int,int)));
    connect(&adc102,SIGNAL(calibPointResult(Sensor*,int,int)),SLOT(onReadCalibPointResult(Sensor*,int,int)));
    connect(&adc102,SIGNAL(updateResult(int,int,int)),SLOT(onUpdateResult(int,int,int)));


    signalMapper  = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);

    network.start(8383);
    connect(&network,SIGNAL(SignalOneMsg(NetClient*,Msg_Head,void*)),SLOT(onOneMsg(NetClient*,Msg_Head,void*)));
    setWindowState(Qt::WindowMaximized);

}

MainWindow::~MainWindow()
{
    delete ui;
}
#include <QTextCodec>

void MainWindow::addItemContent(int row, int column, QString content)

{

      QTableWidgetItem *item = new QTableWidgetItem (content);

      ui->tableWidget->setItem(row, column, item);

}
QString formatGps(GpsDef* gps)
{
    QString gpss;
    if(gps->ns == 'n') gpss.append("n");
    else gpss.append("s");


    gpss += QString("%1").arg(gps->Longitude);

    if(gps->ew == 'e') gpss.append("e");
    else gpss.append("w");


    gpss += QString("%1").arg(gps->Latitude);


    return gpss;
}
void MainWindow::onOneMsg(NetClient * _socket, Msg_Head head, void *arg)
{
    if(head.cmd == 1)
    {
        PointWet* pwt =  (PointWet*)arg;
        qDebug() << "wet" << pwt->wet;

        QTextCodec *codec = QTextCodec::codecForName("GB18030");
        QString duty = codec->toUnicode(pwt->duty);
        //QMessageBox::information(this,"title",duty);
        qDebug() << "wet" << duty;
        int count = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(count+1);

        //QTableWidgetItem *item = new QTableWidgetItem (content);
        QString host = _socket->getSocket()->peerAddress().toString();
        addItemContent(count,0,host);
        addItemContent(count,1,QString("%1").arg(pwt->wet));

        addItemContent(count,2,formatGps(&pwt->gps));
    }
}

void MainWindow::on_actionChagne_triggered()
{
    QMessageBox::about(this,"title","click write parameter");
}
void MainWindow::calibrate_click_zero(int id)
{
    qDebug() << id << "---zero clicked";

    if(adc102.startZeroCalib(id))
    {
        QMessageBox::information(this,"提示","标定成功");
    }
    else
    {
        QMessageBox::information(this,"提示","标定失败");
    }
}
void MainWindow::calibrate_click(int id)
{
    qDebug() << id << "---k clicked";
    bool ok = false;
    bool hand = false;
    if(ui->radioAudo->isChecked()) hand = false;
    if(ui->radioHand->isChecked()) hand = true;

    int weight = ui->tblCalib->item(id,3)->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,"提示","输入重量");
        return;
    }
    if(!adc102.startCalib(hand,id,weight))
    {
        QMessageBox::information(this,"提示","标定失败");
        return;
    }

    QMessageBox::information(this,"提示","标定成功");


}

void MainWindow::onParaReadResult(Para _para)
{
   // ui->cbxDot->setCurrentIndex(_para.dot);
    //ui->edtFullLow->setText(QString("%1").arg(_para.span_low));
    //ui->lbl_display_wet_2->setText(QString("Max1: %1").arg(_para.span_low));
    ui->edtFullHigh->setText(QString("%1").arg(_para.max_weight));
    //ui->lbl_display_wet_3->setText(QString("Max2: %1").arg(_para.span_high));
    ui->cbxDivHigh->setCurrentText(QString("%1").arg(_para.div_high));
    //ui->lbl_display_wet_5->setText(QString("d2: %1").arg(_para.div_high));
    //ui->cbxDivLow->setCurrentText(QString("%1").arg(_para.div_low));
   // ui->lbl_display_wet_4->setText(QString("d1: %1").arg(_para.div_low));

    //ui->edtZeroSpan->setText(QString("%1").arg(_para.zero_track_span));

    //ui->edtHandZeroSpan->setText(QString("%1").arg(_para.hand_zero_span));

    //ui->cbxFilterLvl-(_para.filter_level);
    ui->edtSensorNum->setText(QString("%1").arg(_para.sensorNum));
    //ui->edtVersion->setText(QString("V%1.%2.%3").arg(_para.version/10000).arg((_para.version%10000)/100).arg(_para.version%100));
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

void MainWindow::onUpdateResult(int result, int pos, int total)
{
    switch(result)
    {
    case -1:
        QMessageBox::information(this,tr("error"),tr("start update failed"));
        ui->progressBar->hide();
        break;
    case 1:

        ui->statusBar->showMessage(tr("wait device reset,please reset..."));
        break;
    case 2:
        ui->statusBar->showMessage(tr("wait return key.."));
        break;
    case 3:
        ui->statusBar->showMessage(tr("enter update mode,start send files.."));
        break;
    case 4:
        ui->statusBar->showMessage(tr("ready send file.."));
        ui->progressBar->show();
        if(total > 0)
            ui->progressBar->setValue(pos*100/total);
        break;
    case 5:
        ui->statusBar->showMessage(tr("send file complete.."),2000);
        QMessageBox::information(this,tr("info"),tr("update complete"));
        ui->progressBar->setValue(0);
        ui->progressBar->hide();
        break;
    default:
        ui->statusBar->showMessage(tr("update unkown error"));
        ui->progressBar->hide();
        break;
    }
}
#include <cstdio>
void MainWindow::onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare)
{
    qDebug() << "onweight " << weight;

    char buf[64] = {0,};

    qsnprintf(buf,64,"%d",weight);

    QString ws(buf);

    ui->lblstate->setText(ws);
}
//标定过程....
void MainWindow::onCalibProcessResult(int index, int result)
{

    if(index >= ui->tblCalib->rowCount())
    {
        return;
    }
    float k = (float)result/1000000.0f;
    ui->tblCalib->item(index,2)->setText(QString("%1").arg(k));
}

void MainWindow::onReadCalibPointResult(Sensor* sensors, int num,int weight)
{
    if(ui->tblCalib->rowCount() != num)
    {
        initCalibPoints(num);
    }

    for(int i = 0; i < num; i++)
    {
        QTableWidgetItem *itemMv = ui->tblCalib->item(i,0);
        if(itemMv!=NULL)
        {
            float mv = (float)sensors[i].mv/1000.0f;
            itemMv->setText(QString("%1").arg(mv));
        }
        QTableWidgetItem *itemWt = ui->tblCalib->item(i,1);
        if(itemWt!=NULL)
        {
            itemWt->setText(QString("%1").arg(sensors[i].wt));
        }
//        float k = (float)sensors[i].k/1000000.0f;
//        ui->tblCalib->item(i,2)->setText(QString("%1").arg(k));

    }
    ui->lblstate->setText(QString("%1").arg(weight));


}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    adc102.setSlaveAddr(item->text().toInt());
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_btnSearch_clicked()
{
    QString port = ui->cbxPort->currentText();//QString("COM%1").arg(ui->cbxPort->currentText());
    if(!adc102.startScan(port,ui->cbxBaud->currentText().toInt(),'N',8,1,!ui->cbxFindAll->isChecked()))
    {
        QMessageBox::information(this,tr("error"),tr("uart open failed"));
        return ;
    }
    ui->btnSearch->setEnabled(false);
    ui->listWidget->setEnabled(false);
    ui->listWidget->clear();
    ui->listWidget->setIconSize(QSize(64,64));
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
//    if(!adc102.hasConnect())
//    {
//        if(index != 0)
//        {
//            ui->tabWidget->setCurrentIndex(0);
//            QMessageBox::information(this,tr("info"),tr("please scan device first"));
//        }

//        return ;
//    }

    if(index == 0)
    {

    }
    else if(index == 1)
    {

        //adc102.startReadWeight();
        adc102.startReadPara();
    }
    else if(index == 2)
    {
        adc102.readCalibPoints();
    }
    else if(index == 3)
    {
        //read points first;
        //initCalibPoints(6);

    }
}

void MainWindow::on_btnSave_clicked()
{
    Para p;
    //p.filter_level = ui->cbxFilterLvl->currentIndex();
    p.div_high = ui->cbxDivHigh->currentText().toInt();
    //p.div_low = ui->cbxDivLow->currentText().toInt();
    //p.dot = ui->cbxDot->currentIndex();
    //p.hand_zero_span = ui->edtHandZeroSpan->text().toInt();
//    p.pwr_zero_span  = ui->edtPwrZeroSpan->text().toInt();
//    p.sensor_full_span = ui->edtSensorFullSpan->text().toInt();
//    p.sensor_mv = ui->edtSensorMv->text().toInt();
//    p.slave_addr = ui->edtSlaveAddr->text().toInt();
    p.max_weight = ui->edtFullHigh->text().toInt();
//    p.span_low = ui->edtFullLow->text().toInt();
//    p.stable_span = ui->edtStableSpan->text().toInt();
//    p.unit = ui->cbxUnit->currentIndex();
    //p.zero_track_span = ui->edtZeroSpan->text().toInt();
    p.sensorNum = ui->edtSensorNum->text().toInt();

    //p.adRate = ui->cbxAdRate->currentIndex();
    if(adc102.paraSave(p))
    {
        QMessageBox::information(this,tr("info"),tr("save successful"));
    }
    else
    {
        QMessageBox::information(this,tr("info"),tr("save failed"));
    }
}

void MainWindow::on_btnTare_clicked()
{
    if(!adc102.discardTare())
    {
        QMessageBox::information(this,tr("error"),tr("discard tare failed"));
    }
}

void MainWindow::initCalibPoints(int count)
{
    ui->tblCalib->clear();
    ui->tblCalib->setRowCount(count);
    ui->tblCalib->setColumnCount(6);

    QStringList col_headers;
    col_headers.push_back(tr("mv"));
    col_headers.push_back(tr("实时重量"));
    //col_headers.push_back(tr("ad"));

    col_headers.push_back(tr("k"));

    col_headers.push_back(tr("input"));
    col_headers.push_back(tr("zero"));
    col_headers.push_back(tr("calibrate"));
    //col_headers.push_back("state");
    ui->tblCalib->setHorizontalHeaderLabels(col_headers);

    QStringList row_headers;
    //row_headers<<"0" <<"1" << "2" << "3" << "4" << "5";


    for(int i = 0; i <  count; i++)
    {
        QPushButton* button = NULL;
        if(ui->radioAudo->isChecked())
             button = new QPushButton("记录",ui->tblCalib);
        else
            button = new QPushButton("标重",ui->tblCalib);
        QPushButton* button2 = new QPushButton(tr("zero"),ui->tblCalib);
        //button->setGeometry(0,0,30,20);
        button->setGeometry(0,0,50,30);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        connect(button2, SIGNAL(clicked()), signalMapper2, SLOT(map()));
        signalMapper->setMapping(button, i);
        signalMapper2->setMapping(button2, i);
        ui->tblCalib->setCellWidget(i,5,button);
        ui->tblCalib->setCellWidget(i,4,button2);
        for(int j = 0 ; j < 4; j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tblCalib->setItem(i,j,item);

        }
//        QTableWidgetItem* item = new QTableWidgetItem("");
//        item->setTextAlignment(Qt::AlignHCenter);
//        ui->tblCalib->setItem(i,6,item);
        row_headers.push_back(QString("%1").arg(i+1));

        ui->tblCalib->setColumnWidth(i,120);
        //ui->tblCalib->setH(3, new QTableWidgetItem("参数4"));

    }
    ui->tblCalib->setVerticalHeaderLabels(row_headers);
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(calibrate_click(int)));
    connect(signalMapper2, SIGNAL(mapped(int)),
                this, SLOT(calibrate_click_zero(int)));

}

void MainWindow::on_actionEnglish_triggered()
{
   QTranslator translator;
   bool b = false;

   b = translator.load(QCoreApplication::applicationDirPath() + "/cn.qm");
   _app.installTranslator(&translator);

}

void MainWindow::on_actionChinese_triggered()
{
   QTranslator translator;
   bool b = false;
   b = translator.load(QCoreApplication::applicationDirPath() + "/cn.qm");
   _app.removeTranslator(&translator);

}

void MainWindow::on_btnAddr_clicked()
{
   QListWidgetItem* sel = ui->listWidget->currentItem();
   if(sel == NULL)
   {
        QMessageBox::information(this,tr("info"),tr("please select one device"));
        return;
   }
   quint16 oldAddr = sel->text().toInt();
   quint16 newAddr = ui->edtAddr->text().toInt();

   if(adc102.modifyAddr(oldAddr,newAddr))
   {
        QMessageBox::information(this,tr("info"),tr("modify address successful"));
   }
   else
   {

       QMessageBox::information(this,tr("error"),tr("modify address failed"));
   }
}

void MainWindow::on_btnGN_clicked()
{
    if(!adc102.changeGN())
    {
        QMessageBox::information(this,tr("error"),tr("change groos net failed"));
    }
}

void MainWindow::on_btnUpdate_clicked()
{
    QString file = ui->edtFile->text();
    QFile f(file);
    if(!f.exists())
    {
        QMessageBox::information(this,tr("error"),tr("file do not exist"));
        return;
    }
    if(!adc102.reset())
    {
        //自动复位失败，则提示手动复位...,程序还是一直发送空格...
        QMessageBox::information(this,tr("info"),tr("wait device reset,please reset..."));
    }
    if(!adc102.startUpdate(file))
    {
         QMessageBox::information(this,tr("error"),tr("file update failed"));
    }
    else
    {
        //QMessageBox::information(this,tr("info"),tr("wait device reset,please reset..."));
    }
}

void MainWindow::on_btnSelFile_clicked()
{
   QFileDialog *fileDialog = new QFileDialog(this);
   fileDialog->setWindowTitle(tr("Open Image"));
   fileDialog->setDirectory(".");
   //fileDialog->setFilter(tr("Image Files(*.jpg *.png)"));
   if(fileDialog->exec() == QDialog::Accepted) {
           QString path = fileDialog->selectedFiles()[0];
           ui->edtFile->setText(path);
           //QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
   } else {
           QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
   }
   delete fileDialog;

}

void MainWindow::on_btnReset_clicked()
{
    adc102.reset();
}

void MainWindow::on_btnZero_clicked()
{

    if(!adc102.setZero())
    {
        QMessageBox::information(this,tr("error"),tr("set zero failed"));
    }
}

void MainWindow::on_btnZoom10_clicked()
{

    if(!adc102.zoom10X())
    {
        QMessageBox::information(this,tr("error"),tr("zomm10x failed"));
    }
}

void MainWindow::on_btnCalibAllZero_clicked()
{
    int num = ui->tblCalib->rowCount();
    if(!adc102.calibAllZero(num))
    {
        QMessageBox::information(this,"error","标定失败，请重试");
        return;
    }
    QMessageBox::information(this,"提示","标定成功");
}

void MainWindow::on_btnCalibAllWt_clicked()
{
    bool ok = false;
    bool hand = false;
    if(ui->radioAudo->isChecked()) hand = false;
    if(ui->radioHand->isChecked()) hand = true;
    std::vector<int> weights;
    if(hand)
    {
        int num = ui->tblCalib->rowCount();

        for(int i = 0; i < num ;i++)
        {
            bool ok = false;

            int weight = ui->tblCalib->item(i,3)->text().toInt(&ok);
            if(!ok)
            {

                QMessageBox::information(this,"错误","输入重量");
                return;
            }

            weights.push_back(weight);
        }
    }
    else
    {
        bool ok = false;

        int weight = ui->tblCalib->item(0,3)->text().toInt(&ok);
        if(!ok)
        {

            QMessageBox::information(this,"错误","输入重量");
            return;
        }

        weights.push_back(weight);
    }

    if(!adc102.calibAllWeight(weights,hand))
    {
        QMessageBox::information(this,"错误","标定失败");
        return;
    }
    QMessageBox::information(this,"提示","标定成功!");
}

void MainWindow::on_btnModifyK_clicked()
{
    int num = ui->tblCalib->rowCount();
    if(num < 1 ) return;
    std::vector<qint32> ks;
    for(int i = 0; i < num; i++)
    {
        bool ok = false;
        float k = ui->tblCalib->item(i,2)->text().toFloat(&ok);
        if(!ok)
        {
            QMessageBox::information(this,"error","输入无效参数，请填写全部系数");
            return;
        }
        int intk = (int)(k*1000000);
        ks.push_back(intk);
    }
    if(!adc102.modifyKs(ks))
    {
        QMessageBox::information(this,"错误","修改失败");
    }
    else
    {
        QMessageBox::information(this,"提示","修改成功");
    }

}


void MainWindow::on_btnCalibK_clicked()
{
    bool ok = false;
    int weight = ui->edtFama->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,"错误","重量错误");
        return;
    }
    if(!adc102.fixScalerK(weight))
    {
        QMessageBox::information(this,"错误","标定失败，请重试!");
        return;
    }
    QMessageBox::information(this,"提示","标定成功!");
}

void MainWindow::on_radioAudo_clicked()
{
    ui->btnCalibAllWt->setText("计算系数");
    int num = ui->tblCalib->rowCount();
    if(num < 1 ) return;
    for(int i = 0; i < num; i++)
    {
        QPushButton* widget = (QPushButton*)ui->tblCalib->cellWidget(i,5);

        if(widget!=NULL)
            widget->setText("记录");
    }
}

void MainWindow::on_radioHand_clicked()
{
    ui->btnCalibAllWt->setText("全部标重");
    int num = ui->tblCalib->rowCount();
    if(num < 1 ) return;
    for(int i = 0; i < num; i++)
    {
        QPushButton* widget = (QPushButton*)ui->tblCalib->cellWidget(i,5);

        if(widget!=NULL)
            widget->setText("标重");
    }
}
