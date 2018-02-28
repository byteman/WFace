#include "mywidget.h"
#include <QHBoxLayout>
MyWidget::MyWidget(int addr,QWidget* parent):
    QFrame(parent),
    m_addr(addr)
{
    m_weight = new QLabel(this);
    m_zero_state = new QLabel(this);
    m_still_state = new QLabel(this);
    m_ng_state = new QLabel(this);
    m_state = new QWidget(this);
    QVBoxLayout *item_layout  = new QVBoxLayout;


    QHBoxLayout *state_layout = new QHBoxLayout;
    state_layout->addWidget(m_zero_state);
    state_layout->addWidget(m_still_state);
    state_layout->addWidget(m_ng_state);
    m_state->setLayout(state_layout);


    item_layout->addWidget(m_weight);
    item_layout->addLayout(state_layout);

    this->setLayout(item_layout);
}


void MyWidget::clearState()
{

}
void MyWidget::DisplayWeight(int addr, int weight, quint16 state, quint16 dot)
{
    double wf = (double)weight;

    QString ws = utils::float2string(wf, dot);

    clearState();
    if(state&1)
    {
        ui->lbl_stable->setText(tr("stable"));
        //strState += " | " + tr("stable  ") ;
    }
    if(state&2)
    {
        ui->lbl_zero->setText(tr("zero"));
       //strState += " | " +tr("zero  ");
    }
    if(state&4)
    {
        ui->lbl_net->setText(tr("net"));
       //strState += " | " +tr("net ");
    }
    if(state&8)
    {
        ui->lbl_upflow->setText(tr("upflow"));
       //strState += " | " +tr("upflow ");
       ws = "------";
    }
    if(state&16)
    {
       //strState += " | " +tr("underflow");
         ui->lbl_underflow->setText(tr("underflow"));
       ws = "------";
    }
    if(state&32)
    {
        ui->lbl_highspan->setText(tr("highspan"));
       //strState += " | " +tr("highspan ");
    }
    if(state&64)
    {
       //strState += " | " +tr("zoom10x ");
        ui->lbl_zoom->setText(tr("zoom10x"));
    }
    if(state&128)
    {
        ui->lbl_menucode->setText(tr("menumode"));
       //strState += " | " +tr("menumode ");
    }
//    if(strState.length() > 0)
//    {
//        strState += " | ";
//        ui->lblstate->setText(strState);
//    }

    ui->lbl_display_wet->setText(ws);
}
