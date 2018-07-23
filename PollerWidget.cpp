#include "PollerWidget.h"
#include "ui_PollerWidget.h"

PollerWidget::PollerWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::PollerWidget)
{
    ui->setupUi(this);

    rtwaveWidget = new WaveWidget(ui->rtplot);
    devices = new MyDevices(36,ui->gbDevices);
    devices->SetMaxSampleNum(cfg.m_max_sample);


    ui->edtSaveTime->setValue(cfg.m_save_time_min);
    qDebug() << QDateTime::currentMSecsSinceEpoch();
    devices->SetUnit(cfg.Unit());
    connect(poller,SIGNAL(weightResult(int,int,quint16,quint16,qint32,qint32)),this,SLOT(onPollWeightResult(int,int,quint16,quint16,qint32,qint32)));
    connect(poller,SIGNAL(timeout(int)),this,SLOT(onPollTimeout(int)));


}

PollerWidget::~PollerWidget()
{
    delete ui;
}


void PollerWidget::onPollTimeout(int addr)
{
    if(devices!=NULL)
    {
        devices->Timeout(addr);
        if(addr >= devices->GetEndAddr())
        {
            rtwaveWidget->DisplayAllChannel(true);
        }
    }
}

void PollerWidget::onPollWeightResult(int addr, int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare)
{
    if(devices!=NULL){
        devices->DisplayWeight(addr,weight,state,dot);
        rtwaveWidget->AppendData(addr,utils::int2float(weight,dot));
        if(addr >= devices->GetEndAddr())
        {
            rtwaveWidget->DisplayAllChannel(true);
        }
        //rtwaveWidget->DisplayAllChannel(true);
    }
}
void PollerWidget::SetReadTimeout(int index,int count)
{

    if(index == 0) index = 1000/(count*10);
    else if(index == 1) index = 1000/(count*5);
    else if(index == 2) index = 1000/(count);
    else if(index == 3) index = 1000/(count*0.2);
    else if(index == 4) index = 1000/(count*0.1);
    poller->setReadInterval(index);

}
void PollerWidget::on_btnSetAddr_clicked()
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
            SetReadTimeout(ui->cbxAcqSpan->currentIndex(),count);
            poller->setAddrSpan(startAddr,count);
            devices->SetDeviceNum(startAddr,count);
            devices->SetUnit(cfg.Unit());
            for(int i  = 0; i <  count; i++){
                AlarmInfo aif;
                cfg.GetAlarmSetting(i+startAddr, aif);
                devices->SetAlarmSetting(i+startAddr,aif.index,aif.value);
            }

            rtwaveWidget->SetChannel(startAddr,count);
            rtwaveWidget->Clear();
            m_time = QTime::currentTime();
        }

}

void PollerWidget::on_btnSaveWave_clicked()
{
    devices->SaveWave();
    rtwaveWidget->Clear();
    m_time = QTime::currentTime();
}

void PollerWidget::on_btnClear_clicked()
{
    rtwaveWidget->Clear();
}

void PollerWidget::on_cbxAcqSpan_currentIndexChanged(int index)
{
    bool ok = false;
    qint8 count = ui->edtAddrCount->text().toInt(&ok);
    if(ok)
    {
        SetReadTimeout(index,count);
    }
}

void PollerWidget::on_edtSaveTime_valueChanged(int arg1)
{
    if(arg1 != cfg.m_save_time_min){
      cfg.SetSaveTime(arg1);
    }
}
