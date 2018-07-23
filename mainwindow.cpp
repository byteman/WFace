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
#include "WorkerWidgetManager.h"
//#pragma execution_character_set("utf-8")
static QString unit="g";
static bool scan = false;
void MainWindow::RegisterSlotPara(void)
{
    qRegisterMetaType<Para>("Para");
    qRegisterMetaType<RegCmd>("RegCmd");
    //qRegisterMetaType<QList<int>>("ads");
    qRegisterMetaType<QList<float>>("ks");
}
MainWindow::MainWindow(QApplication &app,QWidget *parent) :
    QMainWindow(parent),
    _app(app),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    WorkerWidgetManager::get()->setChannelUI(ui->grpChannel);
    RegisterSlotPara();
    this->startTimer(500);
    initUI();
}

/**
 * @brief QStringLiteral("我的祖国我") 这样来显示中文.
 * 编码方式要选择utf-8 bom
 */
void MainWindow::initUI()
{

    //pressed = false;
    m_select_addr = 1;
    //ui->lblunit->setText(cfg.Unit());



    this->setWindowTitle(cfg.m_title);


    on_comboBox_currentIndexChanged(0);
    if(!cfg.isAdmin())
    {
        hideForGuest();
    }



}
void MainWindow::hideForGuest()
{

    ui->tabWidget->setTabEnabled(3,false);
    ui->tabWidget->setTabEnabled(4,false);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
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
        if(name != "dumy")
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
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    m_select_addr = item->text().toInt();
    reader.setDeviceAddr(m_select_addr);
    ui->tabWidget->setCurrentIndex(1);
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

//    if(index == 0)
//    {
//        changeHandler("scan",false);
//    }
//    else if(index == 1)
//    {
//        changeHandler("weight");
//    }
//    else if(index == 2)
//    {
//        traversalControl(ui->grpParas->children());
//        loadLocalParam();
//        changeHandler("para");
//    }
//    else if(index == 3)
//    {
//        clearCalib();
//        changeHandler("calib");

//    }
//    else if(index == 4)
//    {
//        clearCornCalib(true);
//        changeHandler("corn");
//    }
//    else if(index == 5)
//    {

//        changeHandler("poll");

//    }
//    else if(index == 6)
//    {
//        changeHandler("dumy");
//        waveWidget->Clear();
//        ReadWaveList();
//    }
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



void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->edtAddr->setText(item->text());
}



void MainWindow::timerEvent(QTimerEvent *)
{
    int rx = 0,tx = 0;
    reader.get_rx_tx(rx,tx);
    QString msg = QString("Addr:%1 TX:%2|RX:%3 ").arg(reader.getCurrentDeviceAddr()).arg(tx).arg(rx);
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
            //on_btnSaveWave_clicked();
        }

    }

}



//void MainWindow::on_btnWriteK_clicked()
//{
//    QList<float> ks;
//    int rows = ui->tblCornFix->rowCount();

//    int num = 0;
//    for(int i = 0; i < rows; i++)
//    {

//        QTableWidgetItem* item = ui->tblCornFix->item(i,1);
//        if(item!=NULL)
//        {
//            bool ok = false;
//            QString sk = item->text();
//            float k = sk.toFloat(&ok);
//            if(!ok)
//            {
//                QString err= item->text()+QString(tr(" format_err"));
//                QMessageBox::information(this,tr("error"),err);

//                return;
//            }
//            ks.push_back(k);
//            num++;
//        }
//    }

//    corn->setKs(ks);
//}

//}


void MainWindow::on_actionReset_triggered()
{

}
void MainWindow::on_btnNetConn_clicked()
{
#if 0
    if(!reader.isConnectd())
    {
        //QString port = ui->cbxPort->currentText();//QString("COM%1").arg(ui->cbxPort->currentText());

        if(!reader.open(ui->edtHost->text(),ui->edtPort->text()))
        {
            QMessageBox::information(this,tr("error"),tr("设备连接失败"));
            return ;
        }
        cfg.SaveNetParam(ui->edtHost->text(),ui->edtPort->text().toInt());
        ui->btnNetConn->setText(tr("断开"));
    }
    else
    {
        reader.close();
        ui->btnNetConn->setText(tr("连接"));
    }
#endif
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    WorkerWidgetManager::get()->selectBus(index);
}
