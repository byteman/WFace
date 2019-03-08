#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfig)
{
    ui->setupUi(this);
    ui->chkAnalog->setChecked(config.IsModulesEnable("analogfix"));
    ui->chkCalib->setChecked(config.IsModulesEnable("calibrate"));
    ui->chkCornFix->setChecked(config.IsModulesEnable("cornfix"));
    ui->chkWave->setChecked(config.IsModulesEnable("realtime_wave"));
    ui->chkWaveData->setChecked(config.IsModulesEnable("history_wave"));
    ui->edtTitle->setText(config.m_title);
    if(config.m_lang=="zh"){
        ui->rbZh->setChecked(true);
    }else if(config.m_lang=="en"){
        ui->rbEn->setChecked(true);
    }
    if(config.m_commu_type==1){
        ui->rbUart->setChecked(true);
    }else if(config.m_commu_type==2){
        ui->rbNet->setChecked(true);
    }

    ui->chkDebug->setChecked(config.m_is_debug);
    ui->chkAdmin->setChecked(config.m_admin);
    ui->chkUartAuto->setChecked(config.m_uart_auto);
}

DialogConfig::~DialogConfig()
{
    delete ui;
}

void DialogConfig::on_btnOk_clicked()
{
    this->accept();
}

void DialogConfig::on_btnCancel_clicked()
{
    this->reject();
}
