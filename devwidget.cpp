#include "devwidget.h"
#include "ui_devwidget.h"
#include "utils.h"
DevWidget::DevWidget(int addr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWidget),
    m_addr(addr),
    m_timeout(3)
{
    ui->setupUi(this);
    ui->lbl_addr->setText(tr("Address") + ":" + QString("%1").arg(addr));
    this->startTimer(1000);
}



DevWidget::~DevWidget()
{
    delete ui;
}

void DevWidget::clearState()
{
    ui->lbl_ng->clear();
    ui->lbl_still->clear();
    ui->lbl_zero->clear();
}
void DevWidget::DisplayWeight(int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);
    m_timeout = 3;
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
       ws = "------";
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


void DevWidget::timerEvent(QTimerEvent *)
{
    if(m_timeout > 0)
    {
        m_timeout--;
    }
    if(m_timeout<=0){
        clearState();
        ui->lbl_weight->setText("");
    }
}
