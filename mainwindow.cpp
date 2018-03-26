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
#include "utils.h"

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
    //QFile file(":/mystyle.txt");
    //file.open(QFile::ReadOnly);
    qDebug() << QDateTime::currentMSecsSinceEpoch();
#if 1
    qDebug() << "mainwindow thread-id:" << QThread::currentThreadId();
    //QByteArray res = file.readAll();

    pressed = false;
    m_select_addr = 1;
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
    scaner->setTimeOut(cfg.m_scan_timeout,cfg.m_read_timeout);
    weight = new WeightHandler(&reader);
    calib = new CalibHandler(&reader);
    para = new ParaHandler(&reader);
    corn = new CornHandler(&reader);
    poller = new PollerHandler(&reader);
    poller->setTimeOut(cfg.m_poll_timeout,cfg.m_read_timeout);
    handlers["scan"] = scaner;
    handlers["weight"] = weight;
    handlers["calib"] = calib;
    handlers["para"] = para;
    handlers["corn"] = corn;

    handlers["poll"] = poller;

    connect(scaner,SIGNAL(scanResult(int,int)),this,SLOT(onScanResult(int,int)));
    connect(weight,SIGNAL(weightResult(int,quint16,quint16,qint32,qint32)),this,SLOT(onWeightResult(int,quint16,quint16,qint32,qint32)));
    connect(weight,SIGNAL(weightParaReadResult(quint16,quint16,quint32,quint32,int)),this,SLOT(onWeightParaRead(quint16,quint16,quint32,quint32,int)));

    connect(calib,SIGNAL(calibReadResult(int,qint32,qint32,int)),this,SLOT(onReadCalibPointResult(int,int,int,int)));
    connect(calib,SIGNAL(calibParaResult(quint32,quint32)),this,SLOT(onReadCalibParam(quint32,quint32)));
    connect(calib,SIGNAL(OperationResult(RegCmd)),this,SLOT(onRegOperResult(RegCmd)));
    connect(calib,SIGNAL(chanADReadResult(QList<float>)),this,SLOT(calibADReadResult(QList<float>)));

    connect(para,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    connect(para,SIGNAL(paraWriteResult(int)),this,SLOT(onParaWriteResult(int)));

    connect(corn,SIGNAL(chanADReadResult(QList<float>)),this,SLOT(chanADReadResult(QList<float>)));
    connect(corn,SIGNAL(chanKReadResult(int,QList<float>)),this,SLOT(chanKReadResult(int,QList<float>)));
    connect(corn,SIGNAL(OperationResult(RegCmd)),this,SLOT(onRegOperResult(RegCmd)));

    connect(poller,SIGNAL(weightResult(int,int,quint16,quint16,qint32,qint32)),this,SLOT(onPollWeightResult(int,int,quint16,quint16,qint32,qint32)));
    connect(poller,SIGNAL(timeout(int)),this,SLOT(onPollTimeout(int)));

    //waveDlg = new DialogWave(this,1);
//    connect(waveDlg,SIGNAL(accepted()),this,SLOT(onAccept()));
//    connect(waveDlg,SIGNAL(finished(int)),this,SLOT(onFinished(int)));
//    connect(waveDlg,SIGNAL(saveWave()),this,SLOT(onSaveWave()));


    initAdList();
    clearState();
    devices = new MyDevices(32,ui->gbDevices);
    devices->SetMaxSampleNum(cfg.m_max_sample);
    //connect(devices,SIGNAL(WaveFull()),this,SLOT(onSaveWave()));
    waveWidget = new WaveWidget(ui->widget);
    rtwaveWidget = new WaveWidget(ui->rtplot,1);
    ui->edtSaveTime->setValue(cfg.m_save_time_min);
    qDebug() << QDateTime::currentMSecsSinceEpoch();
#endif
}
void MainWindow::initAdList()
{
    adlist.push_back(ui->lblAD1);
    adlist.push_back(ui->lblAD2);
    adlist.push_back(ui->lblAD3);
    adlist.push_back(ui->lblAD4);
    for(int i = 0; i < adlist.size();i++)
    {
        adlist[i]->clear();
    }
}

void MainWindow::clearState()
{
    ui->lbl_stable->clear();
    ui->lbl_highspan->clear();
    ui->lbl_menucode->clear();
    ui->lbl_net->clear();
    ui->lbl_underflow->clear();
    ui->lbl_upflow->clear();
    ui->lbl_zero->clear();
    ui->lbl_zoom->clear();
}
//角差标定界面显示的实时AD
void MainWindow::chanADReadResult(QList<float> chanAD)
{
    QTableWidgetItem *item = NULL;

    for(int i = 0; i < chanAD.size();i++)
    {
        item = ui->tblCornFix->item(i,0);
        if(item!=NULL)
            item->setText(QString("%1").arg(chanAD[i]));
    }

}
//标定界面显示的实时AD
void MainWindow::calibADReadResult(QList<float> chanAD)
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
//K系数读取结果.
void MainWindow::chanKReadResult(int sensor, QList<float> chanK)
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
        if(name != "dummy")
        {
           reader.setDeviceAddr(m_select_addr);
        }
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
void MainWindow::corn_calibrate_click(int id)
{
    qDebug() << id << "---clicked";
    corn->calib(id+1);
//    int weight = ui->tblCornFix->item(id,1)->text().toInt();
//    calib->calibSet(id,weight,0);
}
QString format2(int value, int dot)
{

    QString svalue = QString("%1").arg(value);
    if((svalue.size() > dot) )
    {
        if(dot > 0)
        {
            int index = svalue.size() - dot;
            svalue.insert(index,".");
        }

    }
    else
    {
        int n =  dot - svalue.size();
        QString s = QString("0.%1").arg(QString().fill('0',n));
        svalue.prepend(s);
    }
    return svalue;
}
QString MainWindow::formatValue(int value, int dot)
{
    QString str;
    str = format2(value,dot);

    return str;
}
void MainWindow::onParaReadResult(Para _para)
{
    ui->cbxDot->setCurrentIndex(_para.dot);
    //ui->edtFullLow->setText(QString("%1").arg(formatValue(_para.span_low,_para.dot)));


    ui->lbl_display_wet_4->setText(QString("Mid: %1").arg(formatValue(_para.span_low,_para.dot)));
    ui->edtFullHigh->setText(QString("%1").arg(formatValue(_para.span_high,_para.dot)));
    ui->lbl_display_wet_2->setText(QString("Max: %1").arg(formatValue(_para.span_high,_para.dot)));
    ui->cbxDivHigh->setCurrentText(QString("%1").arg(_para.div_high));
    ui->lbl_display_wet_5->setText(QString("d2: %1").arg(_para.div_low));
    //ui->cbxDivLow->setCurrentText(QString("%1").arg(_para.div_low));
    ui->lbl_display_wet_3->setText(QString("d1: %1").arg(_para.div_high));
    //ui->cbxUnit->setCurrentIndex(_para.unit);
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
    ui->lblunit->setText(tr("N"));
    ui->edtZeroSpan->setText(QString("%1").arg(_para.zero_track_span));
    ui->edtStableSpan->setText(QString("%1").arg(_para.stable_span));
    ui->edtHandZeroSpan->setText(QString("%1").arg(_para.hand_zero_span));

    //ui->edtSlaveAddr->setText(QString("%1").arg(_para.slave_addr));
    ui->edtPwrZeroSpan->setText(QString("%1").arg(_para.pwr_zero_span));
    ui->cbxFilterLvl->setCurrentIndex(_para.filter_level);
    //ui->cbxAdRate->setCurrentIndex(_para.adRate);

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

void MainWindow::onPollTimeout(int addr)
{
    if(devices!=NULL)
    {
        devices->Timeout(addr);
    }
}

void MainWindow::onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);

    clearState();
    if(state&1)
    {
        ui->lbl_stable->setText(tr("stable"));
        //strState += " | " + tr("stable  ") ;
    }
    if(state&2)
    {
        ui->lbl_zero->setText(tr("zero"));
       //strState += " | " +tr("zero  ");
    }
    if(state&4)
    {
        ui->lbl_net->setText(tr("net"));
       //strState += " | " +tr("net ");
    }
    if(state&8)
    {
        ui->lbl_upflow->setText(tr("upflow"));
       //strState += " | " +tr("upflow ");
       ws = "------";
    }
    if(state&16)
    {
       //strState += " | " +tr("underflow");
         ui->lbl_underflow->setText(tr("underflow"));
       ws = "------";
    }
    if(state&32)
    {
        ui->lbl_highspan->setText(tr("highspan"));
       //strState += " | " +tr("highspan ");
    }
    if(state&64)
    {
       //strState += " | " +tr("zoom10x ");
        ui->lbl_zoom->setText(tr("zoom10x"));
    }
    if(state&128)
    {
        ui->lbl_menucode->setText(tr("menumode"));
       //strState += " | " +tr("menumode ");
    }
//    if(strState.length() > 0)
//    {
//        strState += " | ";
//        ui->lblstate->setText(strState);
//    }

    ui->lbl_display_wet->setText(ws);
}

void MainWindow::onPollWeightResult(int addr, int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare)
{
    if(devices!=NULL){
        devices->DisplayWeight(addr,weight,state,dot);
        rtwaveWidget->AppendData(addr,utils::int2float(weight,dot));
        rtwaveWidget->DisplayAllChannel(true);
    }
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

void MainWindow::onReadCalibPointResult(int index, int weight, int ad ,int dot)
{

    QTableWidgetItem *itemAd = ui->tblCalib->item(index,0);
    if(itemAd!=NULL)
    {
        itemAd->setText(QString("%1").arg(ad));
    }
    QTableWidgetItem *itemWt = ui->tblCalib->item(index,1);
    if(itemWt!=NULL)
    {
        itemWt->setText(QString("%1").arg(formatValue(weight,dot)));
    }

}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    m_select_addr = item->text().toInt();
    reader.setDeviceAddr(m_select_addr);
    ui->tabWidget->setCurrentIndex(1);
}
//继续下一步标定
bool MainWindow::nextCalib(int index)
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
bool MainWindow::finishEndCalib()
{
    EnableAllCalibButton(false);
    ui->btnStopCalib->setEnabled(false);
    ui->btnStartCalib->setEnabled(true);
    corn->ReadParam();
    QMessageBox::information(this,tr("corn_calib_title"),tr("stop_calib_msg"));
    return true;
}
void MainWindow::onRegOperResult(RegCmd cmd)
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
                    corn->ReadParam();
                }

                QMessageBox::information(this,tr("info"),tr("save_ok"));


            }
        }
        else if((cmd.reg_addr == REG_2B_SENSOR_NUM))
        {

        }
        else if(cmd.reg_addr == REG_2B_AUTO_CORN)
        {

            if(cmd.error != REG_ERROR_OK)
            {
                QMessageBox::information(this,tr("corn_calib_title"),tr("corn_calib_fail_msg"));
            }
            else if(cmd.reg_value[0] == 0)
            {
                //启动成功,请继续点击标定按键
                nextCalib(0);


            }else if(cmd.reg_value[0] == 1)
            {
                nextCalib(1);

            }else if(cmd.reg_value[0] == 2)
            {
                nextCalib(2);

            }else if(cmd.reg_value[0] == 3)
            {
                nextCalib(3);

            }else if(cmd.reg_value[0] == 4)
            {
                nextCalib(4);
            }else if(cmd.reg_value[0] == 5)
            {
                finishEndCalib();
            }
        }

    }


}

void MainWindow::onWeightParaRead(quint16 div_high, quint16 div_low, quint32 full_high, quint32 full_low, int dot)
{

    ui->lbl_display_wet_4->setText(QString("Mid: %1").arg(formatValue(full_low,dot)));
    ui->lbl_display_wet_2->setText(QString("Max: %1").arg(formatValue(full_high,dot)));
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

void MainWindow::onParaWriteResult(int result)
{
    if(0!=result)
    {
        QMessageBox::information(this,tr("info"),tr("save_fail") + QString("code=%1").arg(result));
    }
    else
    {
        QMessageBox::information(this,tr("info"),tr("save_ok"));
    }
    para->reInit();
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
void MainWindow::clearCornCalib(bool clear)
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
    if(!reader.hasConnected() && index != 6)
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
        clearCornCalib(true);
        changeHandler("corn");
    }
    else if(index == 5)
    {

        changeHandler("poll");

    }
    else if(index == 6)
    {
        changeHandler("dumy");
        waveWidget->Clear();
        ReadWaveList();
    }
}
int MainWindow::ReadWaveList()
{
    ui->listWave->clear();
    QStringList files;
    devices->listWaveFiles(files);
    ui->listWave->addItems(files);
    return 0;
}
int MainWindow::toInt(QString txt,int dot,bool *ok)
{
    float tmp = txt.toFloat(ok);
    if(!ok) return false;


    return tmp*pow(10,dot);

}
bool MainWindow::save_param()
{
    Para p;
    bool ok = false;
    p.filter_level = ui->cbxFilterLvl->currentIndex();
    p.div_high = ui->cbxDivHigh->currentText().toInt(&ok);
    if(!ok) return false;
    p.div_low = ui->cbxDivHigh->currentText().toInt();
    if(!ok) return false;
    p.dot = ui->cbxDot->currentIndex();
    if(p.dot==-1) return false;
    p.hand_zero_span = ui->edtHandZeroSpan->text().toInt(&ok);
    if(!ok) return false;
    p.pwr_zero_span  = ui->edtPwrZeroSpan->text().toInt(&ok);
    if(!ok) return false;
    p.stable_span = ui->edtStableSpan->text().toInt(&ok);
    if(!ok) return false;
    p.unit = 0;//ui->cbxUnit->currentIndex();
    if(p.unit==-1) return false;
    p.zero_track_span = ui->edtZeroSpan->text().toInt(&ok);
    if(!ok) return false;
    p.adRate = ui->cbxAdRate->currentIndex();
    if(p.adRate==-1) return false;


    quint8 dot = p.dot;
    if(dot > 6 ) return false;

    p.span_high = toInt(ui->edtFullHigh->text(),dot, &ok);
    if(!ok) return false;

    p.span_low= toInt(ui->edtFullHigh->text(),dot, &ok);

    if(!ok) return false;

    return para->paraSave(p);
}
void MainWindow::on_btnSave_clicked()
{
    if(!save_param())
    {
        QMessageBox::information(this,tr("error"),tr(" format_err"));
    }

}

void MainWindow::onSaveWave()
{
    devices->SaveWave();
    waveWidget->Clear();
    QMessageBox::information(this,tr("info"),tr("save ok"));
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
void MainWindow::initCalibPoints()
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

void MainWindow::on_actionEnglish_triggered()
{
   QTranslator translator;
   bool b = false;

   b = translator.load(QCoreApplication::applicationDirPath() + "/en.qm");
   _app.installTranslator(&translator);
   ui->retranslateUi(this);

}

void MainWindow::on_actionChinese_triggered()
{
   QTranslator translator;
   bool b = false;
   b = translator.load(QCoreApplication::applicationDirPath() + "/cn.qm");
   _app.removeTranslator(&translator);
   ui->retranslateUi(this);

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
       //新地址范围 1-32 之间
       QMessageBox::information(this,tr("info"),tr("address must in 1-32"));
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
//    if(devices!=NULL){
//        devices->DisplayWeight(2,1000,0,0);
//        rtwaveWidget->AppendData(2,1000);
//        rtwaveWidget->DisplayAllChannel(true);
//        //waveDlg->onPollWeightResult(2,1000,0,0,0,0);
//    }
    ui->statusBar->showMessage(msg);
    if(ui->tabWidget->currentIndex() == 5)
    {
        int min = m_time.elapsed() / 60000;
        qDebug() << "min=" << min << " save=" << cfg.m_save_time_min;
        if(cfg.m_save_time_min == 0)
        {
            m_time = QTime::currentTime();
        }
        else if( min >= cfg.m_save_time_min )
        {
            on_btnSaveWave_clicked();
        }

    }

}
void MainWindow::EnableAllCalibButton(bool disable)
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
void MainWindow::EnableCalibButton(int index,bool disable)
{
    QPushButton* btn = (QPushButton*)ui->tblCornFix->cellWidget(index,2);
    if(btn!=NULL)
    {
        btn->setEnabled(disable);
    }
}
void MainWindow::on_btnStartCalib_clicked()
{

   corn->startCalib();

}
void MainWindow::on_btnStopCalib_clicked()
{
   corn->stopCalib();
}

void MainWindow::on_btnReadK_clicked()
{

    corn->ReadParam();
}
void MainWindow::clear_cork_k()
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
void MainWindow::on_btnReadSrs_clicked()
{
    clear_cork_k();
    corn->ReadParam();
}

void MainWindow::on_btnSrsWrite_clicked()
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
                QString err= item->text()+QString(tr(" format_err"));
                QMessageBox::information(this,tr("error"),err);

                return;
            }
            ks.push_back(k);
            num++;
        }
    }

    corn->setKs(ks);
}


void MainWindow::on_tblCalib_cellEntered(int row, int column)
{
    qDebug() << "enter" << row << column;

}

void MainWindow::on_tblCalib_cellChanged(int row, int column)
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

void MainWindow::on_tblCalib_cellPressed(int row, int column)
{

    pressed = true;
}

void MainWindow::on_btnSetAddr_clicked()
{
    if(poller!=NULL){
        bool ok = false;

        qint8 startAddr = ui->edtStartAddr->text().toInt(&ok);
        if(!ok) return;
        qint8 count = ui->edtAddrCount->text().toInt(&ok);
        if(!ok) return;
        if(count<0) count=0;
        if( (startAddr+count) > 32)
        {
            QMessageBox::information(this,tr("info"),tr("error addr span"));
            return;
        }
        int index = ui->cbxAcqSpan->currentIndex();
        if(index == 0) index = 10;
        else if(index == 1) index = 5;
        else index = 1;

        int timeout = (1000000/index) / count;
        if(timeout < 50000) timeout = 50000;
        poller->setTimeOut(timeout,cfg.m_read_timeout);
        poller->setAddrSpan(startAddr,count);
        devices->SetDeviceNum(startAddr,count);
        rtwaveWidget->SetChannel(startAddr,count);
        rtwaveWidget->Clear();
        m_time = QTime::currentTime();
    }
}

void MainWindow::on_btnOpen_clicked()
{

}

#include "mydevices.h"

void MainWindow::on_listWave_itemClicked(QListWidgetItem *item)
{
    ChannelsData wvd;
    devices->LoadWave(item->text(),wvd);
    waveWidget->SetData(wvd);
    waveWidget->DisplayAllChannel(true);
}

void MainWindow::on_btnClearWave_clicked()
{
    waveWidget->Clear();
}

void MainWindow::on_btnSaveWave_clicked()
{
    devices->SaveWave();
    rtwaveWidget->Clear();
    m_time = QTime::currentTime();
}

void MainWindow::on_btnClear_clicked()
{
    rtwaveWidget->Clear();
}

void MainWindow::on_edtSaveTime_valueChanged(int arg1)
{
    cfg.SetSaveTime(arg1);
}
