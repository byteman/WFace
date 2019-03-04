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

    qDebug() << QDateTime::currentMSecsSinceEpoch();
#if 1
    qDebug() << "mainwindow thread-id:" << QThread::currentThreadId();

    pressed = false;
    m_select_addr = 1;

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QSerialPortInfo port;
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName());
    }
    ui->cbxBaud->setCurrentIndex(0);
    ui->scanPb->hide();


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

    connect(para,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    connect(para,SIGNAL(paraWriteResult(int)),this,SLOT(onParaWriteResult(int)));


    connect(poller,SIGNAL(weightResult(int,int,quint16,quint16,qint32,qint32)),this,SLOT(onPollWeightResult(int,int,quint16,quint16,qint32,qint32)));
    connect(poller,SIGNAL(timeout(int)),this,SLOT(onPollTimeout(int)));
    connect(poller,SIGNAL(weightSumResult(int,int,int,int,int)),this,SLOT(onPollWeightSumResult(int,int,int,int,int)));


    devices = new MyDevices(33,ui->gbDevices);
    devices->SetMaxSampleNum(cfg.m_max_sample);
    //connect(devices,SIGNAL(WaveFull()),this,SLOT(onSaveWave()));
    //waveWidget = new WaveWidget(ui->widget);
    //rtwaveWidget = new WaveWidget(ui->rtplot,1);
    //ui->edtSaveTime->setValue(cfg.m_save_time_min);
    qDebug() << QDateTime::currentMSecsSinceEpoch();
    devices->SetUnit(cfg.Unit());
#endif
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


void MainWindow::on_actionChagne_triggered()
{
     reader.write_register(2,99);
}

void MainWindow::onParaReadResult(Para _para)
{
    ui->edtSensorNum->setText(QString("%1").arg(_para.sensor_num));
    ui->edtReadTime->setText(QString("%1").arg(_para.read_time_out));
    ui->edtLimit->setText(QString("%1").arg(_para.limit));
    ui->sbNewAddr->setValue(_para.slaveAddr);

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
        if(addr >= devices->GetEndAddr())
        {
            //rtwaveWidget->DisplayAllChannel(true);
        }
    }
}


void MainWindow::onPollWeightResult(int addr, int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare)
{
    if(devices!=NULL){
        devices->DisplayWeight(addr,weight,state,dot);
        //rtwaveWidget->AppendData(addr,utils::int2float(weight,dot));
//        if(addr >= devices->GetEndAddr())
//        {
//            rtwaveWidget->DisplayAllChannel(true);
//        }
        //rtwaveWidget->DisplayAllChannel(true);
    }
}

void MainWindow::onPollWeightSumResult(int num, int sum1,int sum2, int sum3 ,int sum4)
{
    ui->edtSumKg->setText(QString("%1").arg(sum1));
    ui->edtSum2->setText(QString("%1").arg(sum2));
    ui->edtSum3->setText(QString("%1").arg(sum3));
    ui->edtSum4->setText(QString("%1").arg(sum4));
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    m_select_addr = item->text().toInt();
    reader.setDeviceAddr(m_select_addr);
    reader.setDeviceId(m_select_addr);
    ui->tabWidget->setCurrentIndex(1);
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
        //int id = ui->sbId->text().toInt();
        scaner->init(3,1,1,1,33,!ui->cbxFindAll->isChecked());
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

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //ui->tabWidget->currentWidget()->objectName();
    if(!reader.hasConnected() && index != 3)
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
        //traversalControl(ui->grpParas->children());
        changeHandler("para");
    }

    else if(index == 2)
    {

        changeHandler("poll");

    }
    else if(index == 3)
    {
        changeHandler("dumy");
        //waveWidget->Clear();
        ReadWaveList();
    }
}
int MainWindow::ReadWaveList()
{
    //ui->listWave->clear();
    QStringList files;
    devices->listWaveFiles(files);
    //ui->listWave->addItems(files);
    return 0;
}
int MainWindow::toInt(QString txt,int dot,bool *ok)
{
    float tmp = txt.toFloat(ok);
    if(!ok) return false;


    return tmp*pow(10,dot);

}

void MainWindow::on_btnAddr_clicked()
{

}




void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //ui->edtAddr->setText(item->text());
}



void MainWindow::timerEvent(QTimerEvent *)
{
    int rx = 0,tx = 0;
    //adc102.getRXTX(rx,tx);
    reader.get_rx_tx(rx,tx);
    QString msg = QString("TX:%1|RX:%2 ").arg(tx).arg(rx);

    ui->statusBar->showMessage(msg);
    if(ui->tabWidget->currentIndex() == 5)
    {
        int min = m_time.elapsed() / 60000;
        //qDebug() << "min=" << min << " save=" << cfg.m_save_time_min;
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



void MainWindow::SetReadTimeout(int index,int count)
{

    if(index == 0) index = 1000/(count*10);
    else if(index == 1) index = 1000/(count*5);
    else if(index == 2) index = 1000/(count);
    else if(index == 3) index = 1000/(count*0.2);
    else if(index == 4) index = 1000/(count*0.1);
    poller->setReadInterval(index);

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
        if( (startAddr+count) > 33)
        {
            QMessageBox::information(this,tr("info"),tr("error addr span"));
            return;
        }

        poller->setTimeOut(100000,cfg.m_read_timeout);
        //SetReadTimeout(ui->cbxAcqSpan->currentIndex(),count);
        poller->setAddrSpan(startAddr,count);
        devices->SetDeviceNum(startAddr,count);
        devices->SetUnit(cfg.Unit());
        //rtwaveWidget->SetChannel(startAddr,count);
        //rtwaveWidget->Clear();
        m_time = QTime::currentTime();
    }
}

#include "mydevices.h"

void MainWindow::on_listWave_itemClicked(QListWidgetItem *item)
{
    ChannelsData wvd;
    devices->LoadWave(item->text(),wvd);
    //waveWidget->SetData(wvd);
    //waveWidget->DisplayAllChannel(true);
}

void MainWindow::on_btnClearWave_clicked()
{
    //waveWidget->Clear();
}

void MainWindow::on_btnSaveWave_clicked()
{
    devices->SaveWave();
    //rtwaveWidget->Clear();
    m_time = QTime::currentTime();
}

void MainWindow::on_btnClear_clicked()
{
    //rtwaveWidget->Clear();
}

void MainWindow::on_edtSaveTime_valueChanged(int arg1)
{
    cfg.SetSaveTime(arg1);
}

void MainWindow::on_cbxAcqSpan_currentIndexChanged(int index)
{
    bool ok = false;
    qint8 count = ui->edtAddrCount->text().toInt(&ok);
    if(ok)
    {
        SetReadTimeout(index,count);
    }

}


void MainWindow::on_btnZero_clicked()
{
    if(!confirm()){
        return;
    }
    int addr = ui->sbAddr->value();
    calib->clearZero(addr);
}

void MainWindow::on_btnRead_clicked()
{
    para->reInit();
}

void MainWindow::on_btnCalibZero_clicked()
{
    if(!confirm()){
        return;
    }
    quint8 addr = ui->sbAddr->value();
    calib->calibZero(addr);
}

void MainWindow::on_btnSave_clicked()
{
    Para par;
    par.sensor_num = ui->edtSensorNum->text().toInt();
    par.limit = ui->edtLimit->text().toInt();
    par.read_time_out = ui->edtReadTime->text().toInt();
    par.slaveAddr = ui->sbNewAddr->value();

    para->paraSave(par);

}

void MainWindow::on_btnCalibWgt_clicked()
{
    if(!confirm()){
        return;
    }
    quint8 addr = ui->sbAddr->value();
    calib->calibWgt(addr);
}
QString MainWindow::formatState(SensorState& state)
{
    QString str;
    if(state.StateBit.zero){
        //str+="零位,";
        str += tr("zero");
    }
    if(state.StateBit.still){
        //str+="稳定,";
        str += tr("still");
    }
    if(state.StateBit.soc_err){
        //str+="芯片错误,";
         str += tr("soc");
    }
    if(state.StateBit.overflow){
        //str+="溢出,";
         str += tr("overflow");
    }
    if(state.StateBit.power_zero_overflow){
        //str+="开机零点溢出" + ",";
         str += tr("power overflow");
    }
    if(state.StateBit.sensor_err){
        //str+="传感器故障"+",";
         str += tr("sensor");
    }
    return str;

}
void MainWindow::on_btnQueryError_clicked()
{
    SensorErrInfoList list;
    if(para->queryErrorSensor(list))
    {
        ui->listResults->clear();
        for(int i = 0; i < list.size(); i++)
        {
            QString str;
            str += QString("地址%1").arg(list[i].addr);
            str += formatState(list[i].state);
            ui->listResults->addItem(str);
        }
    }
}

void MainWindow::on_btnQueryChange_clicked()
{
    SensorWgtInfoList list;
    if(para->queryChangeSensor(list))
    {
        ui->listResults->clear();
        for(int i = 0; i < list.size(); i++)
        {
            QString str;
            str += QString("地址%1->").arg(list[i].addr);
            str += " "+formatState(list[i].state);
            str += QString("重量%1").arg(list[i].wgt);
            ui->listResults->addItem(str);
        }
    }

}

void MainWindow::on_btnModifyAddr_clicked()
{
    if(!confirm()){
        return;
    }
    quint8 addr = ui->sbAddr->value();
    if(para->modifyAddr(addr)){
        QMessageBox::information(this,"提示","修改成功");
    }
    else
    {
        QMessageBox::information(this,"错误","修改失败");
    }
}
bool MainWindow::confirm()
{
    return (QMessageBox::Yes == QMessageBox::question(this,"提示","确认操作",QMessageBox::Yes|QMessageBox::No,QMessageBox::No));

}
void MainWindow::on_btnChange_clicked()
{
    if(ui->btnChange->text()=="显示AD"){
        poller->showAD(true);
        ui->btnChange->setText("显示重量");
    }else
    {
         poller->showAD(false);
         ui->btnChange->setText("显示AD");
    }
}

void MainWindow::on_btnSet_clicked()
{
    bool ok = false;
    if(!confirm()){
        return;
    }
    quint8 addr = ui->sbAddr->value();
    qint16 weight = ui->edtSWet->text().toInt(&ok);
    if(ok){
        if(calib->setWeight(addr,weight)){
            QMessageBox::information(this,"提示","修改成功");
        }
        else
        {
            QMessageBox::information(this,"错误","修改失败");
        }
    }else{
        QMessageBox::information(this,"错误","没有填写重量");
    }


}



void MainWindow::on_edtSumNum_textChanged(const QString &arg1)
{
    bool ok = false;

    int num = arg1.toInt(&ok);
    if(ok){
        poller->setSumNum(num);
    }
}
