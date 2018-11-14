#include "devwidget.h"
#include "ui_devwidget.h"
#include "utils.h"
#include <QDebug>
#define MAX_TIMEOUT 3
DevWidget::DevWidget(int addr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWidget),
    m_addr(addr),
    m_timeout(MAX_TIMEOUT),
    m_last_still(false),
    m_zoom(false)
{
    ui->setupUi(this);
    ui->lbl_addr->setText(tr("Address") + ":" + QString("%1").arg(addr));
    clearState();
    SetOnline(true);
    ui->lbl_weight->setText("");
    ui->lbl_still_time->hide();
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
    if(m_timeout <= 0){
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

void DevWidget::SetAlarmSetting(int setting, double value)
{
    m_alarm_index = setting;
    m_alarm_value = value;
}

void DevWidget::Show()
{
    if(m_zoom){

       ui->lbl_weight->setStyleSheet("font-size : 128px");
    }else{
       ui->lbl_weight->setStyleSheet("font-size : 16pt");

    }
    this->show();
}

void DevWidget::Reset()
{
    m_zoom = false;
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
QString DevWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);
    QString wt = ws;
    resetTimeout();
    clearState();

    double wti = utils::int2float(weight,dot);
    //qDebug() << "addr" << m_addr << " wti=" <<wti << " value= " << m_alarm_value << " index=" << m_alarm_index;
    if(m_alarm_index == 0){
        if(wti <= m_alarm_value){
            SetOnline(false);
        }else if(!m_online){
            SetOnline(true);
        }
    }else{
        if(wti > m_alarm_value){
            SetOnline(false);
        }else if(!m_online){
            SetOnline(true);
        }
    }

    if(state&1)
    {
        ui->lbl_still->setText(tr("stable"));
        if(!m_last_still){

           //ui->lbl_still_time->setText(QString("稳定时间:%1").arg(m_still_time.elapsed())) ;
        }
        //strState += " | " + tr("stable  ") ;
    }
    else
    {
        //从稳定到不稳定(从新计算时间)
        if(m_last_still){
            m_still_time = QTime::currentTime();

        }

    }
    m_last_still = state&1?true:false;

    if(state&2)
    {
        ui->lbl_zero->setText(tr("zero"));
       //strState += " | " +tr("zero  ");
    }
    if(state&4)
    {
        ui->lbl_ng->setText(tr("net"));
       //strState += " | " +tr("net ");
    }
    if(state&8)
    {
       // ui->lbl_upflow->setText(tr("upflow"));
       //strState += " | " +tr("upflow ");
       //ws = "------";
    }
    if(state&16)
    {
       //strState += " | " +tr("underflow");
       //  ui->lbl_underflow->setText(tr("underflow"));
       ws = "------";
    }
    if(state&32)
    {
       // ui->lbl_highspan->setText(tr("highspan"));
       //strState += " | " +tr("highspan ");
    }
    if(state&64)
    {
       //strState += " | " +tr("zoom10x ");
        //ui->lbl_zoom->setText(tr("zoom10x"));
    }
    if(state&128)
    {
        //ui->lbl_menucode->setText(tr("menumode"));
       //strState += " | " +tr("menumode ");
    }
    //int unit= (state >>8)&0x7;

//    if(strState.length() > 0)
//    {
//        strState += " | ";
//        ui->lblstate->setText(strState);
//    }

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
    m_online = online;
}


void DevWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug() <<m_addr <<" double click";
    m_zoom=!m_zoom;
    if(m_zoom){

       ui->lbl_weight->setStyleSheet("font-size : 128px");
    }else{
       ui->lbl_weight->setStyleSheet("font-size : 16pt");

    }
    emit onDoubleClick(m_addr,m_zoom);
}
