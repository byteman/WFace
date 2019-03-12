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

    for(int i = 0; i < config.m_params.size(); i++)
    {
        if(config.m_params[i] == "alarm"){
            ui->chkAlarm->setChecked(true);
        }
        else if(config.m_params[i] == "serial"){

            ui->chkSerial->setChecked(true);

        }else if(config.m_params[i] == "version"){
            ui->chkVersion->setChecked(true);
        }else if(config.m_params[i] == "channel"){
            ui->chkChannel->setChecked(true);
        }
        else if(config.m_params[i] == "modifyAddr"){

        }
        else if(config.m_params[i] == "changeView"){
            ui->chkView->setChecked(true);
        }

    }

}

DialogConfig::~DialogConfig()
{
    delete ui;
}

void DialogConfig::on_btnOk_clicked()
{
 /*
    config->setValue("/config/lang",m_lang);
    config->setValue("/config/title",m_title);
    config->setValue("/config/commu",m_commu_type);
    config->setValue("/config/admin",m_admin);
    config->setValue("/config/debug",m_is_debug);
    config->setValue("/uart/auto",m_uart_auto);
    m_modules["measure"] = config->value("modules/measure",true).toBool();
    m_modules["param"] = config->value("modules/param",true).toBool();
    m_modules["calibrate"] = config->value("modules/calibrate",true).toBool();
    m_modules["cornfix"] = config->value("modules/cornfix",true).toBool();
    m_modules["realtime_wave"] = config->value("modules/realtime_wave",true).toBool();
    m_modules["history_wave"] = config->value("modules/history_wave",true).toBool();
    m_modules["analogfix"] = config->value("modules/analogfix",true).toBool();

*/
    if(ui->rbEn->isChecked()){
        config.m_lang="en";
    }else if(ui->rbZh->isChecked()){
        config.m_lang="zh";
    }
    config.m_title = ui->edtTitle->text();
    config.m_admin = ui->chkAdmin->isChecked();
    if(ui->rbUart->isChecked()){
        config.m_commu_type = 1;
    }else if(ui->rbNet->isChecked()){
        config.m_commu_type = 2;
    }
    config.m_is_debug = ui->chkDebug->isChecked();
    config.m_uart_auto = ui->chkUartAuto->isChecked();

    config.m_modules["calibrate"]=ui->chkCalib->isChecked();
    config.m_modules["cornfix"]=ui->chkCornFix->isChecked();
    config.m_modules["realtime_wave"]=ui->chkWave->isChecked();
    config.m_modules["history_wave"]=ui->chkWaveData->isChecked();
    config.m_modules["analogfix"]=ui->chkAnalog->isChecked();

    config.SaveAll();
    this->accept();
}

void DialogConfig::on_btnCancel_clicked()
{
    this->reject();
}

void DialogConfig::on_groupBox_4_clicked()
{

}
