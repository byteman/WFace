#include "devwidget.h"
#include "ui_devwidget.h"
#include "utils.h"
#define MAX_TIMEOUT 3
DevWidget::DevWidget(int addr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWidget),
    m_addr(addr),
    m_timeout(MAX_TIMEOUT),
    m_last_still(false)
{
    ui->setupUi(this);
    ui->lbl_addr->setText(tr("Address") + ":" + QString("%1").arg(addr));
    clearState();
    SetOnline(true);
    ui->lbl_weight->setText("");
}



DevWidget::~DevWidget()
{
    delete ui;
}

void DevWidget::Timeout()
{
    if(m_timeout > 0)
    {
        m_timeout--;
        SetOnline(true);
    }
    if(m_timeout<=0){
        clearState();
        ui->lbl_weight->setText("");
        SetOnline(false);
    }
}

void DevWidget::SetUnit(QString unit)
{
    m_unit = unit;
    ui->lbl_unit->setText(unit);
}

void DevWidget::clearState()
{
    ui->lbl_ng->clear();
    ui->lbl_still->clear();
    ui->lbl_zero->clear();
}
void DevWidget::resetTimeout()
{
    m_timeout = MAX_TIMEOUT;
}
#include "command.h"
QString DevWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);
    QString wt = ws;
    resetTimeout();
    clearState();
    SensorState ss;
    ss.StateValue = state;


    if(ss.StateBit.still)
    {
        ui->lbl_still->setText(tr("stable"));
        //从不稳定到稳定，显示时间
//        if(!m_last_still){

//           ui->lbl_still_time->setText(QString("稳定时间:%1ms").arg(m_still_time.elapsed())) ;
//        }

        //strState += " | " + tr("stable  ") ;
    }
    else
    {
        //从稳定到不稳定(从新计算时间)
//        if(m_last_still){
//            m_still_time = QTime::currentTime();

//        }

    }
    m_last_still = ss.StateBit.still?true:false;
    if(ss.StateBit.zero)
    {
        ui->lbl_zero->setText(tr("zero"));
       //strState += " | " +tr("zero  ");
    }
    if(ss.StateBit.overflow && !ss.StateBit.bit7)
    {
       //ws = "------";
       //strState += " | " +tr("net ");
    }
    if(ss.StateBit.power_zero_overflow)
    {
       ui->lbl_zero->setText("PO");
    }
    if(ss.StateBit.soc_err)
    {
       //strState += " | " +tr("underflow");
       //  ui->lbl_underflow->setText(tr("underflow"));
       ui->lbl_zero->setText("SOC");
    }
    if(ss.StateBit.sensor_err)
    {
       // ui->lbl_highspan->setText(tr("highspan"));
       ui->lbl_zero->setText("S_ERR");
    }
    if(ss.StateBit.bit6){
        ui->lbl_ng->setText("off");
    }

    ui->lbl_weight->setText(ws);
    return wt;

}

void DevWidget::SetOnline(bool online)
{
    if(online){
        ui->lbl_addr->setStyleSheet("background-color: green;");
    }else{
        ui->lbl_addr->setStyleSheet("background-color: red;");
    }
}
