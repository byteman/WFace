#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include <QMessageBox>
#include <qdebug.h>
#include <QSignalMapper>
#include <QTranslator>
#include <QSerialPortInfo>
#include "pcomm.h"
#include <QFileDialog>
#include <QFile>
#include <cstdio>
static QString unit="g";
static bool scan = false;
MainWindow::MainWindow(QApplication &app,QWidget *parent) :
    QMainWindow(parent),
    _app(app),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<Para>("Para");
    qRegisterMetaType<RegCmd>("RegCmd");
    //qRegisterMetaType<QList<int>>("ads");
    qRegisterMetaType<QList<float>>("ks");

    this->startTimer(500);
    initUI();
}
void MainWindow::initUI()
{
    QFile file(":/mystyle.txt");
    file.open(QFile::ReadOnly);


    QByteArray res = file.readAll();


    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QSerialPortInfo port;
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName());
    }
    ui->cbxBaud->setCurrentIndex(1);
    ui->scanPb->hide();

    initCalibPoints();
    initCornFixChan();
    reader.start(100);
    scaner = new ScanHandler(&reader);
    weight = new WeightHandler(&reader);
    calib = new CalibHandler(&reader);
    para = new ParaHandler(&reader);
    corn = new CornHandler(&reader);

    handlers["scan"] = scaner;
    handlers["weight"] = weight;
    handlers["calib"] = calib;
    handlers["para"] = para;
    handlers["corn"] = corn;
    connect(scaner,SIGNAL(scanResult(int,int)),this,SLOT(onScanResult(int,int)));
    connect(weight,SIGNAL(weightResult(int,quint16,quint16,qint32,qint32)),this,SLOT(onWeightResult(int,quint16,quint16,qint32,qint32)));
    connect(weight,SIGNAL(weightParaReadResult(quint16,quint16,quint32,quint32)),this,SLOT(onWeightParaRead(quint16,quint16,quint32,quint32)));

    connect(calib,SIGNAL(calibReadResult(int,qint32,qint32)),this,SLOT(onReadCalibPointResult(int,int,int)));
    connect(calib,SIGNAL(calibParaResult(quint32,quint32)),this,SLOT(onReadCalibParam(quint32,quint32)));
    connect(calib,SIGNAL(OperationResult(RegCmd)),this,SLOT(onRegOperResult(RegCmd)));

    connect(para,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    connect(para,SIGNAL(paraWriteResult(bool)),this,SLOT(onParaWriteResult(bool)));

    connect(corn,SIGNAL(chanADReadResult(QList<float>)),this,SLOT(chanADReadResult(QList<float>)));
    connect(corn,SIGNAL(chanKReadResult(int,QList<float>)),this,SLOT(chanKReadResult(int,QList<float>)));
    connect(corn,SIGNAL(OperationResult(RegCmd)),this,SLOT(onRegOperResult(RegCmd)));

}
//AD读取结果
void MainWindow::chanADReadResult(QList<float> chanAD)
{
    for(int i = 0; i < chanAD.size();i++)
    {
        ui->tblCornFix->item(i,0)->setText(QString("%1").arg(chanAD[i]));
    }

}
//K系数读取结果.
void MainWindow::chanKReadResult(int sensor, QList<float> chanK)
{
    for(int i = 0; i < chanK.size();i++)
    {
        ui->tblCornFix->item(i,1)->setText(QString("%1").arg(chanK[i]));
    }
    ui->edtSensorNum->setText(QString("%1").arg(sensor));
}
void MainWindow::changeHandler(QString name,bool start)
{
    QMapIterator<QString,CmdHandler*> i(handlers);
    while (i.hasNext()) {
        i.next();
        i.value()->stop();
        //cout << i.key() << ": " << i.value() << endl;
    }
    if(start)
    {
        if(handlers.contains(name))
            handlers[name]->startRun();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadCalibParam(quint32 sensorMv, quint32 sensorFullSpan)
{
    ui->edtSensorFullSpan->setText(QString("%1").arg(sensorFullSpan));
    ui->edtSensorMv->setText(QString("%1").arg(sensorMv));
}

void MainWindow::on_actionChagne_triggered()
{
     reader.write_register(2,99);
}

void MainWindow::calibrate_click(int id)
{
    qDebug() << id << "---clicked";
    int weight = ui->tblCalib->item(id,1)->text().toInt();
    calib->calibSet(id,weight,0);
}
void MainWindow::corn_calibrate_click(int id)
{
    qDebug() << id << "---clicked";
    corn->calib(id+1);
//    int weight = ui->tblCornFix->item(id,1)->text().toInt();
//    calib->calibSet(id,weight,0);
}
void MainWindow::onParaReadResult(Para _para)
{
    ui->cbxDot->setCurrentIndex(_para.dot);
    ui->edtFullLow->setText(QString("%1").arg(_para.span_low));
    ui->lbl_display_wet_4->setText(QString("Mid: %1").arg(_para.span_low));
    ui->edtFullHigh->setText(QString("%1").arg(_para.span_high));
    ui->lbl_display_wet_2->setText(QString("Max: %1").arg(_para.span_high));
    ui->cbxDivHigh->setCurrentText(QString("%1").arg(_para.div_high));
    ui->lbl_display_wet_5->setText(QString("d2: %1").arg(_para.div_low));
    ui->cbxDivLow->setCurrentText(QString("%1").arg(_para.div_low));
    ui->lbl_display_wet_3->setText(QString("d1: %1").arg(_para.div_high));
    ui->cbxUnit->setCurrentIndex(_para.unit);
    ui->edtSerial->setText(QString("%1").arg(_para.serial));
    if(_para.unit == 0) {
        ui->lblunit->setText("kg");
        unit = "kg";
    }
    else if(_para.unit == 1) {
        ui->lblunit->setText("g");
        unit = "g";
    }
    else if(_para.unit == 2) {
        unit = "t";
        ui->lblunit->setText("t");
    }
    ui->edtZeroSpan->setText(QString("%1").arg(_para.zero_track_span));
    ui->edtStableSpan->setText(QString("%1").arg(_para.stable_span));
    ui->edtHandZeroSpan->setText(QString("%1").arg(_para.hand_zero_span));

    //ui->edtSlaveAddr->setText(QString("%1").arg(_para.slave_addr));
    ui->edtPwrZeroSpan->setText(QString("%1").arg(_para.pwr_zero_span));
    ui->cbxFilterLvl->setCurrentIndex(_para.filter_level);
    ui->cbxAdRate->setCurrentIndex(_para.adRate);

    ui->edtVersion->setText(QString("V%1.%2.%3").arg(_para.version/10000).arg((_para.version%10000)/100).arg(_para.version%100));
}



void MainWindow::onScanResult(int type,int addr)
{
    if(type == SCAN_FIND)
    {
        QString title = QString("%1").arg(addr);
        QListWidgetItem* item = new QListWidgetItem(QIcon(":/monitor.png"),title);
        ui->listWidget->addItem(item);

        scan = true;
    }
    else if(type == SCAN_COMPLETE)
    {
        ui->btnSearch->setEnabled(true);
        ui->btnSearch->setText(tr("BusScan"));
        ui->listWidget->setEnabled(true);
        scan = false;
        ui->scanPb->hide();
    }
    else if(type == SCAN_PROGRASS)
    {
        ui->scanPb->show();
        ui->scanPb->setValue(addr);
    }
}


void MainWindow::onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare)
{
    double wf = (double)weight;
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf/10);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf/100);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf/1000);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf/10000);
            break;
        default:
            qsnprintf(buf,64,"%d",weight);
            break;
    }
    QString ws(buf);


    QString strState = "";
    //state = 0xFF;
    if(state&1)
    {
        strState += " | " + tr("stable  ") ;
    }
    if(state&2)
    {
       strState += " | " +tr("zero  ");
    }
    if(state&4)
    {
       strState += " | " +tr("net ");
    }
    if(state&8)
    {
       strState += " | " +tr("upflow ");
       ws = "------";
    }
    if(state&16)
    {
       strState += " | " +tr("underflow ");
       ws = "------";
    }
    if(state&32)
    {
       strState += " | " +tr("highspan ");
    }
    if(state&64)
    {
       strState += " | " +tr("zoom10x ");
    }
    if(state&128)
    {
       strState += " | " +tr("menumode ");
    }
    if(strState.length() > 0)
    {
        strState += " | ";
        ui->lblstate->setText(strState);
    }

    ui->lbl_display_wet->setText(ws);
}
//标定过程....
void MainWindow::onCalibProcessResult(int index, int result)
{
    if(result > 0)
    {
        ui->statusBar->showMessage(QString("left time %1S").arg(result));
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

void MainWindow::onReadCalibPointResult(int index, int weight, int ad)
{
    QTableWidgetItem *itemAd = ui->tblCalib->item(index,0);
    if(itemAd!=NULL)
    {
        itemAd->setText(QString("%1").arg(ad));
    }
    QTableWidgetItem *itemWt = ui->tblCalib->item(index,1);
    if(itemWt!=NULL)
    {
        itemWt->setText(QString("%1").arg(weight));
    }

}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    reader.setDeviceAddr(item->text().toInt());
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::onRegOperResult(RegCmd cmd)
{
    qDebug() << "onRegOperResult " << "reg=" << cmd.reg_addr << " num=" << cmd.reg_num << " " << cmd.isRead;
    if(cmd.reg_addr == REG_CALIB)
    {
        if(cmd.error == REG_ERROR_OK)
        {
            QMessageBox::information(this,tr("info"),"标定成功");
            return ;
        }
        else
        {
            QMessageBox::information(this,tr("error"),"标定失败");
            return ;
        }
    }
    else if(!cmd.isRead)
    {
        if( (cmd.reg_addr == REG_FULL_SPAN)||
            (cmd.reg_addr == REG_2B_SENSOR_NUM) ||
                (cmd.reg_addr == REG_4B_CORN_K))
        {
            if(cmd.error != REG_ERROR_OK)
            {
                QMessageBox::information(this,"提示","保存失败");
            }
            else
            {
                QMessageBox::information(this,tr("info"),"保存成功");
            }
        }
        else if(cmd.reg_addr == REG_2B_AUTO_CORN)
        {
            if(cmd.error != REG_ERROR_OK)
            {
                QMessageBox::information(this,"提示","操作失败");
            }
            else if(cmd.reg_value[0] == 0)
            {
                QMessageBox::information(this,"提示","启动成功");
            }else if(cmd.reg_value[0] == 1)
            {
                QMessageBox::information(this,"提示","标定成功");
            }else if(cmd.reg_value[0] == 2)
            {
                QMessageBox::information(this,"提示","标定成功");
            }else if(cmd.reg_value[0] == 3)
            {
                QMessageBox::information(this,"提示","标定成功");
            }else if(cmd.reg_value[0] == 4)
            {
                QMessageBox::information(this,"提示","标定成功");
            }else if(cmd.reg_value[0] == 5)
            {
                QMessageBox::information(this,"提示","标定结束");
            }
        }

    }


}

void MainWindow::onWeightParaRead(quint16 div_high, quint16 div_low, quint32 full_high, quint32 full_low)
{
    qDebug() << "onWeightParaRead";
    ui->lbl_display_wet_4->setText(QString("Mid: %1").arg(full_low));
    ui->lbl_display_wet_2->setText(QString("Max: %1").arg(full_high));
    ui->lbl_display_wet_5->setText(QString("d2: %1").arg(div_low));
    ui->lbl_display_wet_3->setText(QString("d1: %1").arg(div_high));
}

void MainWindow::on_btnSearch_clicked()
{
    if(!scan)
    {
        QString port = ui->cbxPort->currentText();//QString("COM%1").arg(ui->cbxPort->currentText());
        if(!reader.open(port,ui->cbxBaud->currentText().toInt(),'N',8,1))
        {
            QMessageBox::information(this,tr("error"),tr("uart open failed"));
            return ;
        }
        scaner->init(3,1,1,33,!ui->cbxFindAll->isChecked());
        scaner->start();
        ui->btnSearch->setText(tr("StopSearch"));
        ui->listWidget->setEnabled(false);
        ui->listWidget->clear();
        ui->listWidget->setIconSize(QSize(64,64));
        scan = true;
    }
    else
    {
        scaner->stop();
    }


}

void MainWindow::onParaWriteResult(bool ok)
{
    if(!ok)
    {
        QMessageBox::information(this,"提示","保存失败");
    }
    else
    {
        QMessageBox::information(this,tr("info"),"保存成功");
    }
}
void MainWindow::traversalControl(const QObjectList& q)
{
    foreach(QObject* obj,q)
    {
           QString class_name=obj->metaObject()->className();

           if(class_name=="QLineEdit")
           {
               QLineEdit* le=(QLineEdit*)obj;

               le->setText("");
           }
           else if(class_name=="QComboBox")
           {
               QComboBox* cbx=(QComboBox*)obj;

               cbx->setCurrentText("");
           }
    }

}
void MainWindow::clearCornCalib()
{
    int rows = ui->tblCornFix->rowCount();
    int cols = ui->tblCornFix->columnCount();
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QTableWidgetItem* item = ui->tblCalib->item(i,j);
            if(item!=NULL)
                item->setText("");
        }
    }
    ui->edtSensorNum->setText("");

}
void MainWindow::clearCalib()
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
    ui->edtSensorFullSpan->setText("");
    ui->edtSensorMv->setText("");
}
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(!reader.hasConnected())
    {
        if(index != 0)
        {
            ui->tabWidget->setCurrentIndex(0);
            QMessageBox::information(this,tr("info"),tr("please scan device first"));
        }

        return ;
    }

    if(index == 0)
    {
        changeHandler("scan",false);
    }
    else if(index == 1)
    {
        changeHandler("weight");
    }
    else if(index == 2)
    {
        traversalControl(ui->grpParas->children());
        changeHandler("para");
    }
    else if(index == 3)
    {
        clearCalib();
        changeHandler("calib");

    }
    else if(index == 4)
    {
        clearCornCalib();
        changeHandler("corn");
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
    p.span_high = ui->edtFullHigh->text().toInt();
    p.span_low = ui->edtFullLow->text().toInt();
    p.stable_span = ui->edtStableSpan->text().toInt();
    p.unit = ui->cbxUnit->currentIndex();
    p.zero_track_span = ui->edtZeroSpan->text().toInt();
    p.adRate = ui->cbxAdRate->currentIndex();
    para->paraSave(p);
//    {
//        QMessageBox::information(this,tr("info"),tr("save successful"));
//    }
//    else
//    {
//        QMessageBox::information(this,tr("info"),tr("save failed"));
//    }
}

void MainWindow::on_btnTare_clicked()
{

    if(!weight->discardTare())
    {
        QMessageBox::information(this,tr("error"),tr("discard tare failed"));
    }
}
void MainWindow::initCornFixChan()
{
    ui->tblCornFix->setRowCount(4);
    ui->tblCornFix->setColumnCount(3);
    QStringList col_headers;
//    col_headers.push_back(tr("channel"));
    col_headers.push_back(tr("ad"));
    col_headers.push_back(tr("k"));
    col_headers.push_back(tr("Operation"));
    ui->tblCornFix->setHorizontalHeaderLabels(col_headers);

    QStringList row_headers;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i <=  3; i++)
    {
        QPushButton* button = new QPushButton(tr("calib"),ui->tblCornFix);
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
        row_headers.push_back(QString("%1").arg(i));

    }
    ui->tblCornFix->setVerticalHeaderLabels(row_headers);
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(corn_calibrate_click(int)));
    ui->btnStopCalib->setEnabled(false);

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

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i <=  5; i++)
    {
        QPushButton* button = new QPushButton(tr("calib"),ui->tblCalib);
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
        row_headers.push_back(QString("%1").arg(i));

    }
    ui->tblCalib->setVerticalHeaderLabels(row_headers);
    connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(calibrate_click(int)));
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
   if(newAddr > 32 || newAddr < 1)
   {
       QMessageBox::information(this,tr("info"),"新地址范围 1-32 之间");
       return;
   }
   reader.setDeviceAddr(oldAddr);
   reader.write_register(REG_ADDR,newAddr);
   reader.setDeviceAddr(newAddr);
   if(1==reader.read_registers(REG_ADDR,1,&oldAddr))
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
    if(!weight->changeGN())
    {
        QMessageBox::information(this,tr("error"),tr("change groos net failed"));
    }
}

void MainWindow::on_btnReset_clicked()
{
    //adc102.reset();
}

void MainWindow::on_btnZero_clicked()
{

    if(!weight->setZero())
    {
        QMessageBox::information(this,tr("error"),tr("set zero failed"));
    }
}

void MainWindow::on_btnZoom10_clicked()
{

    if(!weight->zoom10X())
    {
        QMessageBox::information(this,tr("error"),tr("zomm10x failed"));
    }
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->edtAddr->setText(item->text());
}

void MainWindow::on_btnSensorWrite_clicked()
{
    quint32 sensor_full_span; // 传感器总量程（所有传感器量程和）
    quint32 sensor_mv;

    sensor_full_span = ui->edtSensorFullSpan->text().toInt();
    sensor_mv = ui->edtSensorMv->text().toInt();

    calib->savePara(sensor_full_span,sensor_mv);

}


void MainWindow::timerEvent(QTimerEvent *)
{
    int rx = 0,tx = 0;
    //adc102.getRXTX(rx,tx);
    reader.get_rx_tx(rx,tx);
    QString msg = QString("TX:%1|RX:%2 ").arg(tx).arg(rx);

    ui->statusBar->showMessage(msg);
}

void MainWindow::on_btnStartCalib_clicked()
{
    corn->startCalib();
}

void MainWindow::on_pushButton_2_clicked()
{
    corn->stopCalib();
}

void MainWindow::on_pushButton_5_clicked()
{

}

void MainWindow::on_btnReadK_clicked()
{
    corn->ReadParam();
}

void MainWindow::on_btnReadSrs_clicked()
{
    corn->ReadParam();
}

void MainWindow::on_btnSrsWrite_clicked()
{
    bool ok = false;
    int num = ui->edtSensorNum->text().toInt(&ok);

    corn->setSensorNum(num);
}

void MainWindow::on_btnWriteK_clicked()
{
    QList<float> ks;
    int rows = ui->tblCornFix->rowCount();

    int num = 0;
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
                QString err= QString("[%1]格式错误").arg(item->text());
                QMessageBox::information(this,"错误",err);
                return;
            }
            ks.push_back(k);
            num++;
        }
    }

    corn->setKs(ks);
}

void MainWindow::on_btnStopCalib_clicked()
{
    ui->btnStartCalib->setEnabled(true);
}
