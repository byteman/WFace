#include "WeightWidget.h"
#include "ui_WeightWidget.h"
#include "utils.h"
#include <QDebug>
//1. 设置Reader
//2. 切换Reader的handler为自己 . 有个管理器类，负责切换CmdHandler


WeightWidget::WeightWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WeightWidget)
{
    ui->setupUi(this);
    InitUI();
}
void WeightWidget::clearState()
{
    ui->lbl_stable->clear();
    ui->lbl_highspan->clear();
    ui->lbl_menucode->clear();
    ui->lbl_net->clear();
    ui->lbl_underflow->clear();
    ui->lbl_upflow->clear();
    ui->lbl_zero->clear();
    ui->lbl_zoom->clear();
}
void WeightWidget::InitUI(void)
{
    clearState();
    weight = new WeightHandler(reader);
    connect(weight,SIGNAL(weightResult(int,quint16,quint16,qint32,qint32)),this,SLOT(onWeightResult(int,quint16,quint16,qint32,qint32)));
    connect(weight,SIGNAL(weightParaReadResult(quint16,quint16,quint32,quint32,int)),this,SLOT(onWeightParaRead(quint16,quint16,quint32,quint32,int)));

}
void WeightWidget::onWeightParaRead(quint16 div_high, quint16 div_low, quint32 full_high, quint32 full_low, int dot)
{

    ui->lbl_display_wet_4->setText(QString("Mid: %1").arg(utils::formatValue(full_low,dot)));
    ui->lbl_display_wet_2->setText(QString("Max: %1").arg(utils::formatValue(full_high,dot)));
    ui->lbl_display_wet_5->setText(QString("d2: %1").arg(div_low));
    ui->lbl_display_wet_3->setText(QString("d1: %1").arg(div_high));
}

void WeightWidget::onWeightResult(int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare)
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
    ui->lblunit->setText(cfg.Unit());
    ui->lbl_display_wet->setText(ws);
}
WeightWidget::~WeightWidget()
{
    delete ui;
}

void WeightWidget::on_btnGN_clicked()
{
    if(!weight->changeGN())
    {
        QMessageBox::information(this,tr("error"),tr("change groos net failed"));
    }
}

void WeightWidget::on_btnTare_clicked()
{

    if(!weight->discardTare())
    {
        QMessageBox::information(this,tr("error"),tr("discard tare failed"));
    }
}

void WeightWidget::on_btnZoom10_clicked()
{
    if(!weight->zoom10X())
    {
        QMessageBox::information(this,tr("error"),tr("zomm10x failed"));
    }
}

void WeightWidget::on_btnZero_clicked()
{
    if(!weight->setZero())
    {
        QMessageBox::information(this,tr("error"),tr("set zero failed"));
    }
}
