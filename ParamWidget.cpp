#include "ParamWidget.h"
#include "ui_ParamWidget.h"
#include "utils.h"
#include "WorkerWidgetManager.h"
#include <QDebug>
ParamWidget::ParamWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ParamWidget)
{
    ui->setupUi(this);

    ui->edtUnit->setText(cfg.Unit());
    connect(para,SIGNAL(paraReadResult(Para)),this,SLOT(onParaReadResult(Para)));
    connect(para,SIGNAL(paraWriteResult(int)),this,SLOT(onParaWriteResult(int)));
    //traversalControl()
}

ParamWidget::~ParamWidget()
{
    delete ui;
}
void ParamWidget::loadLocalParam()
{
    ui->edtUnit->setText(cfg.m_unit);

    AlarmInfo aif;
    int addr = WorkerWidgetManager::get()->getReader()->getCurrentDeviceAddr();
    //int addr = reader.getCurrentDeviceAddr();
    if(addr != -1){
        cfg.GetAlarmSetting(addr,aif);
        ui->cbxAlarmSetting->setCurrentIndex(aif.index);
        ui->edtAlarmValue->setText(QString("%1").arg(aif.value));
    }

}
bool ParamWidget::save_param()
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
    p.pwr_zero_span  = 0;//ui->edtPwrZeroSpan->text().toInt(&ok);
    //if(!ok) return false;
    p.stable_span = ui->edtStableSpan->text().toInt(&ok);
    if(!ok) return false;
    p.unit = 0;//ui->cbxUnit->currentIndex();
    if(p.unit==-1) return false;
    p.zero_track_span = ui->edtZeroSpan->text().toInt(&ok);
    if(!ok) return false;
    p.adRate = 0;//ui->cbxAdRate->currentIndex();
    //if(p.adRate==-1) return false;


    quint8 dot = p.dot;
    if(dot > 6 ) return false;

    p.span_high = utils::toInt(ui->edtFullHigh->text(),dot, &ok);
    if(!ok) return false;

    p.span_low= utils::toInt(ui->edtFullHigh->text(),dot, &ok);

    if(!ok) return false;

    return para->paraSave(p);
}

void ParamWidget::onParaWriteResult(int result)
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
void ParamWidget::onParaReadResult(Para _para)
{
    ui->cbxDot->setCurrentIndex(_para.dot);
    //ui->edtFullLow->setText(QString("%1").arg(formatValue(_para.span_low,_para.dot)));


    //ui->lbl_display_wet_4->setText(QString("Mid: %1").arg(utils::formatValue(_para.span_low,_para.dot)));
    ui->edtFullHigh->setText(QString("%1").arg(utils::formatValue(_para.span_high,_para.dot)));
    //ui->lbl_display_wet_2->setText(QString("Max: %1").arg(utils::formatValue(_para.span_high,_para.dot)));
    ui->cbxDivHigh->setCurrentText(QString("%1").arg(_para.div_high));
    //ui->lbl_display_wet_5->setText(QString("d2: %1").arg(_para.div_low));
    //ui->cbxDivLow->setCurrentText(QString("%1").arg(_para.div_low));
    //ui->lbl_display_wet_3->setText(QString("d1: %1").arg(_para.div_high));
    //ui->cbxUnit->setCurrentIndex(_para.unit);
    ui->edtSerial->setText(QString("%1").arg(_para.serial));
    if(_para.unit == 0) {
        //ui->lblunit->setText("kg");
        //unit = "kg";
    }
    else if(_para.unit == 1) {
        //ui->lblunit->setText("g");
        //unit = "g";
    }
    else if(_para.unit == 2) {
        //unit = "t";
        //ui->lblunit->setText("t");
    }
    ui->edtUnit->setText(cfg.Unit());

    ui->edtZeroSpan->setText(QString("%1").arg(_para.zero_track_span));
    ui->edtStableSpan->setText(QString("%1").arg(_para.stable_span));
    ui->edtHandZeroSpan->setText(QString("%1").arg(_para.hand_zero_span));

    //ui->edtSlaveAddr->setText(QString("%1").arg(_para.slave_addr));
    //ui->edtPwrZeroSpan->setText(QString("%1").arg(_para.pwr_zero_span));
    ui->cbxFilterLvl->setCurrentIndex(_para.filter_level);
    //ui->cbxAdRate->setCurrentIndex(_para.adRate);

    ui->edtVersion->setText(QString("V%1.%2.%3").arg(_para.version/10000).arg((_para.version%10000)/100).arg(_para.version%100));
}

void ParamWidget::on_btnSave_clicked()
{
    if(!save_param())
    {
        QMessageBox::information(this,tr("error"),tr(" format_err"));
    }
    cfg.SaveUnit(ui->edtUnit->text());
    bool ok = false;



    int addr = WorkerWidgetManager::get()->getReader()->getCurrentDeviceAddr();
    int index = ui->cbxAlarmSetting->currentIndex();
    double value = ui->edtAlarmValue->text().toDouble(&ok);

    if(addr == -1){
        qDebug() << "on_btnSave_clicked invalid address";
        return;
    }
    if(!ok){
        qDebug() << "on_btnSave_clicked can not get value";
        return;
    }
    cfg.SaveAlarmSetting(addr, index, value);
    //devices->SetAlarmSetting(addr,index,value);
    //devices->SetUnit(cfg.Unit());

}


void ParamWidget::hidenForGuest()
{
    ui->edtSerial->hide();
    ui->label_15->hide();
    ui->edtVersion->hide();
    ui->label_16->hide();

    ui->cbxDot->setEnabled(false);
    ui->edtUnit->setEnabled(false);
    ui->edtFullHigh->setEnabled(false);
}
