#include "devwidget.h"
#include "ui_devwidget.h"
#include "utils.h"
#define MAX_TIMEOUT 3
DevWidget::DevWidget(int addr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWidget),
    m_addr(addr),
    m_timeout(MAX_TIMEOUT)
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
void DevWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);
    resetTimeout();
    clearState();
    if(state&1)
    {
        ui->lbl_still->setText(tr("stable"));
        //strState += " | " + tr("stable  ") ;
    }
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
//    if(strState.length() > 0)
//    {
//        strState += " | ";
//        ui->lblstate->setText(strState);
//    }

    ui->lbl_weight->setText(ws);

}

void DevWidget::SetOnline(bool online)
{
    if(online){
        ui->lbl_addr->setStyleSheet("background-color: green;");
    }else{
        ui->lbl_addr->setStyleSheet("background-color: red;");
    }
}
