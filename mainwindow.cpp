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


    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QSerialPortInfo port;
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName());
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

    connect(&m_timer,SIGNAL(timeout()),SLOT(onTimerHandle()));
    signalMapper  = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);

    network.start(8889);
    connect(&network,SIGNAL(SignalOneMsg(NetClient*,Msg_Head,void*)),SLOT(onOneMsg(NetClient*,Msg_Head,void*)));
    connect(&network,SIGNAL(SignalUpdateEvent(NetClient*,int, UpdateEvtPara)), SLOT(onUpdateEvent(NetClient*,int, UpdateEvtPara)));
    connect(&network,SIGNAL(SignalNewClient(NetClient*)), SLOT(onNewClient(NetClient*)));
    connect(&network,SIGNAL(SignalRemoveClient(int)), SLOT(onRemoveClient(int)));

    setWindowState(Qt::WindowMaximized);
    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
        ui->tableWidget->setColumnWidth(i,200);

    QList<QGroupBox*> list;
    list.push_back(ui->grpParas1);
    list.push_back(ui->grpParas2);
    //cpm.load("params.json",list);
    //GPSMark("40.047669,116.313082");
    //ui->webView->load();
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

void MainWindow::onTimerHandle()
{
    qDebug() << "timeout cmd = " << m_read_cmds.size();
    if(m_read_cmds.size() > 0)
    {
        network.readPara(m_read_cmds.front());
        m_timer.stop();
        m_timer.start(5000);
    }
    else
    {
        m_timer.stop();
    }
}
void MainWindow::addItem(QString id)
{
    QString title = QString("%1").arg(id);
    QListWidgetItem* item = new QListWidgetItem(QIcon(":/monitor.png"),title);
    ui->listWidget->addItem(item);
}

void MainWindow::onNewClient(NetClient *client)
{
    QString id = "unkown";
    if(client!=NULL)
        id = client->getID();
    addItem(id);
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
QString formatTime(DateDef& d)
{
    return QString("%1-%2-%3 %4:%5:%6").arg(2000+d.year).arg(d.month).arg(d.day).arg(d.hour).arg(d.min).arg(d.sec);
}
#include <QDateTime>

bool MainWindow::checkAck(QString para,int oper,int value)
{
    if(oper == OPER_HOST_WRITE_DEV)
    {
        //写数据的回应.
        if(value != 0)
        {
            QMessageBox::information(this,"title","write gps style failed");

        }
        return false;
    }
    return true;
}
void MainWindow::removeCmd(int cmd)
{
    QList<int> removes;
    m_timer.stop();
    if(m_read_cmds.size() == 0)
    {

        return;
    }

    for(int i = 0; i < m_read_cmds.size(); i++)
    {
        if(m_read_cmds[i] == cmd)
        {
            removes.append(i);
        }
    }
    for(int i = 0; i < removes.size(); i++)
    {
        m_read_cmds.removeAt(removes[i]);
    }
    if(m_read_cmds.size() > 0)
    {
        network.readPara(m_read_cmds.front());
        m_timer.start(5000);
    }
}
void MainWindow::onOneMsg(NetClient * _socket, Msg_Head head, void *arg)
{

    if(head.cmd == CMD_DEV2HOST_ONE_WEIGHT)
    {
        PointWet* pwt =  (PointWet*)arg;
        qDebug() << "wet" << pwt->wet;

        QTextCodec *codec = QTextCodec::codecForName("GB18030");
        char buffer[32] = {0,};
        memcpy(buffer,pwt->duty,DUTY_LEN);
        QString duty = codec->toUnicode(buffer);
        memset(buffer,0,32);
        memcpy(buffer,pwt->license_plate,LICENSE_LEN);
        QString card = codec->toUnicode(buffer);
        //QMessageBox::information(this,"title",duty);
        qDebug() << "wet" << duty;
        int count = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(count+1);

        //QTableWidgetItem *item = new QTableWidgetItem (content);
        QString host = _socket->getSocket()->peerAddress().toString();

        int i = 0;

        addItemContent(count,i++,card);
        addItemContent(count,i++,host);

        addItemContent(count,i++,formatGps(&pwt->gps));
        addItemContent(count,i++,formatTime(pwt->wet_date));
        QDateTime qdt = QDateTime::currentDateTime();

        addItemContent(count,i++,qdt.toString("yyyy-MM-dd hh:mm:ss"));
        addItemContent(count,i++,QString("%1").arg(pwt->wet));
    }
    else if(head.cmd == CMD_VER)
    {
        int ver = *((int*)arg);
        if(head.oper == OPER_HOST_WRITE_DEV)
        {

            return;
        }
        removeCmd(CMD_VER);
        QString vers = QString("ver%1.%2.%3").arg((ver>>16)&0xFF).arg((ver>>8)&0xFF).arg((ver)&0xFF);
        ui->edtVersion->setText(vers);
    }
    else if(head.cmd == CMD_GPS)
    {
        quint8 gps=  *((quint8*)arg);

        if(head.oper == OPER_HOST_WRITE_DEV)
        {
            //写数据的回应.
            if(gps != 0)
            {
                QMessageBox::information(this,"title","write gps style failed");

            }
            else
            {
                QMessageBox::information(this,"title","write gps style ok");
            }
            return;
        }
        removeCmd(CMD_GPS);
        if(gps == 0 || gps == 1)
        {
            //ui->cbxGps->setCurrentIndex(gps);
        }
    }
    else if(head.cmd == CMD_GPS_REPORT_TIME)
    {

        quint8 gpstime=  *((quint8*)arg);
        if(head.oper == OPER_HOST_WRITE_DEV)
        {
            //写数据的回应.
            if(gpstime != 0)
            {
                QMessageBox::information(this,"title","write gps time failed");

            }
            else
            {
                QMessageBox::information(this,"title","write gps time ok");
            }
            return;
        }
        removeCmd(CMD_GPS_REPORT_TIME);
        ui->edtGpsTime->setText(QString("%1").arg(gpstime));
    }
    else if(head.cmd == CMD_DEV_REPORT_TIME)
    {
        quint8 devtime=  *((quint8*)arg);
        if(head.oper == OPER_HOST_WRITE_DEV)
        {
            //写数据的回应.
            if(devtime != 0)
            {
                QMessageBox::information(this,"title","write dev time failed");

            }
            else
                 QMessageBox::information(this,"title","write dev time ok");
            return;
        }
        removeCmd(CMD_DEV_REPORT_TIME);
        ui->edtDevTime->setText(QString("%1").arg(devtime));
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

    QString ip=QString("%1.%2.%3.%4").arg((_para.serverIp>>24)&0xff).arg((_para.serverIp>>16)&0xff).arg((_para.serverIp>>8)&0xff).arg(_para.serverIp&0xff);
    ui->edtServerIp->setText(ip);
    ui->edtServerPort->setText(QString("%1").arg(_para.serverPort));
    ui->edtFullHigh->setText(QString("%1").arg(_para.max_weight));

    ui->cbxDivHigh->setCurrentText(QString("%1").arg(_para.div_high));

    ui->edtSensorNum->setText(QString("%1").arg(_para.sensorNum));
    ui->edtACC->setText(QString("%1").arg(_para.acc_delay));
    ui->edtXZ->setText(QString("%1").arg(_para.limit_weight));
    QTextCodec *codec = QTextCodec::codecForName("GB18030");


    QString plate(codec->toUnicode((char*)_para.plate));

    ui->edtPlate->setText(plate);

    QString simCard((char*)_para.simCard);
    //ui->edtSimCard->setText(simCard);
    int ver = _para.version;
    //ui->edtVersion->setText(QString("%1").arg(_para.version));
    QString vers = QString("ver%1.%2.%3").arg((ver>>16)&0xFF).arg((ver>>8)&0xFF).arg((ver)&0xFF);
    ui->edtVersion->setText(vers);


    ui->edtGpsTime->setText(QString("%1").arg(_para.gps_report));
    ui->edtDevTime->setText(QString("%1").arg(_para.dev_report));

    ui->edtDelay->setText(QString("%1").arg(_para.delay_time));
    QString calibTime = QString("%1-%2-%3 %4:%5:%6").arg(_para.calib_datetime[0]+2000).arg(_para.calib_datetime[1]).arg(_para.calib_datetime[2]).arg(_para.calib_datetime[3]).arg(_para.calib_datetime[4]).arg(_para.calib_datetime[5]);

    ui->edtCalibTime->setText(calibTime);
}



void MainWindow::onScanResult(int type,int addr)
{
    if(type == 0)
    {
        addItem(QString("%1").arg(addr));
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
    int id = 0;
    bool ok = false;
    id = (item->text().toInt(&ok));
    if(ok && id <= 32)
    {
        adc102.setSlaveAddr(item->text().toInt());
        isUart = true;
    }
    else
    {
        curDev = item->text();
        isUart = false;
        network.setCurrentClient(curDev);
    }
    ui->tabWidget->setCurrentIndex(1);

}

void MainWindow::onUpdateEvent(NetClient *_client, int evt, UpdateEvtPara para)
{
    switch(evt)
    {
        case UEVT_START:
            ui->updateLog->setText("ready update");
            break;
        case UEVT_DATA:
            {
                ui->progressBar->show();
                if(para.total_pkg_index > 0)
                {
                    ui->progressBar->setValue(para.cur_pkg_index*100/para.total_pkg_index);
                }
            }
            break;
        case UEVT_END:
            
            break;
    case UEVT_OK:
            break;
    default:break;
    }
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

    if(index == 0)
    {

    }
    else if(index == 1)
    {

        if(isUart)
        {
            traversalControl(ui->grpParas1->children());
            traversalControl(ui->grpParas2->children());
            adc102.startReadPara();
        }
        else
        {
            //network.readPara(CMD_VER);
            m_read_cmds.clear();
            m_read_cmds.append(CMD_VER);
            m_read_cmds.append(CMD_GPS);
            m_read_cmds.append(CMD_GPS_REPORT_TIME);
            m_read_cmds.append(CMD_DEV_REPORT_TIME);
            network.readPara(CMD_VER);
            m_timer.start(5000);

        }

    }
    else if(index == 2)
    {
        if(isUart)
        {

            adc102.readCalibPoints();
        }
    }
    else if(index == 3)
    {
        //read points first;
        //initCalibPoints(6);

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
void MainWindow::on_btnSave_clicked()
{
    Para p;

    QString plate = ui->edtPlate->text();
    //QString s = UTF82GBK(plate);
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");



    QByteArray b;//ip.toIPv4Address(); = s.to
    b = gbk->fromUnicode(plate);
//b.append(s);
    for(int i = 0; i < b.size(); i++)
    {
        p.plate[i] = b[i];
        if(i>=10) break;
    }
    QHostAddress ip(ui->edtServerIp->text());

    p.serverIp = ip.toIPv4Address();
    p.serverPort = ui->edtServerPort->text().toInt();

    p.div_high = ui->cbxDivHigh->currentText().toInt();

    p.max_weight = ui->edtFullHigh->text().toInt();

    p.sensorNum = ui->edtSensorNum->text().toInt();

    p.delay_time = ui->edtDelay->text().toInt();
    p.gps_report = ui->edtGpsTime->text().toInt();
    p.dev_report = ui->edtDevTime->text().toInt();
    p.acc_delay = ui->edtACC->text().toInt();
    p.limit_weight = ui->edtXZ->text().toInt();

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
//    if(!adc102.reset())
//    {
//        //自动复位失败，则提示手动复位...,程序还是一直发送空格...
//        QMessageBox::information(this,tr("info"),tr("wait device reset,please reset..."));
//    }
//    if(!adc102.startUpdate(file))
//    {
//         QMessageBox::information(this,tr("error"),tr("file update failed"));
//    }
//    else
//    {
//        //QMessageBox::information(this,tr("info"),tr("wait device reset,please reset..."));
//    }
    network.startUpdate(curDev,file);
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
    if(isUart)
        adc102.reset();
    else
    {
        network.reset();
    }

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

void MainWindow::onRemoveClient(int)
{
    if(closed) return;
    ui->listWidget->clear();
    curDev.clear();
    QStringList devs;
    network.getOnLineClients(devs);
    ui->tabWidget->setCurrentIndex(0);
    for(int i = 0 ; i <devs.size();i++)
        addItem(devs[i]);
    m_timer.stop();

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


void MainWindow::closeEvent(QCloseEvent *)
{
    closed = true;
}

void MainWindow::on_edtVersion_returnPressed()
{
    qDebug() << "return";
}

void MainWindow::on_edtDevTime_returnPressed()
{
    qDebug() << "return";
    bool ok = false;
    quint8 val = ui->edtDevTime->text().toInt(&ok);
    if(!ok)
    {
        return ;
    }
    QByteArray data;
    data.append(val);

    network.writePara(CMD_DEV_REPORT_TIME,data);
}

void MainWindow::on_edtGpsTime_returnPressed()
{
    qDebug() << "return";
    bool ok = false;
    quint8 val = ui->edtGpsTime->text().toInt(&ok);
    if(!ok)
    {
        return ;
    }
    QByteArray data;
    data.append(val);

    network.writePara(CMD_GPS_REPORT_TIME,data);
}

void MainWindow::on_cbxGps_currentIndexChanged(int index)
{

//    quint8 val = ui->cbxGps->currentIndex();

//    QByteArray data;
//    data.append(val);

//    network.writePara(CMD_GPS,data);
}

void MainWindow::on_pushButton_clicked()
{
    if(isUart)
    {
        adc102.startReadPara();
    }
    else
    {
        m_read_cmds.clear();
        m_read_cmds.append(CMD_VER);
        m_read_cmds.append(CMD_GPS);
        m_read_cmds.append(CMD_GPS_REPORT_TIME);
        m_read_cmds.append(CMD_DEV_REPORT_TIME);
        network.readPara(CMD_VER);
        m_timer.start(5000);

    }
}

void MainWindow::on_btnQuitCalib_clicked()
{
    if(adc102.stopCalib())
    {
        QMessageBox::information(this,"提示","结束标定完成!");
    }
}
