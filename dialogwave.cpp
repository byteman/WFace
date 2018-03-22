#include "dialogwave.h"
#include "ui_dialogwave.h"

DialogWave::DialogWave(QWidget *parent,int num) :
    QDialog(parent),
    m_channel(num),
    ui(new Ui::DialogWave)
{
    ui->setupUi(this);
    m_waveWdg = new WaveWidget(ui->widget,num);
}

DialogWave::~DialogWave()
{
    delete ui;
}

void DialogWave::SetChannel(int start, int num)
{

    m_waveWdg->SetChannel(start,num);
    m_waveWdg->Clear();
    this->show();
}

void DialogWave::onPollWeightResult(int addr, int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare)
{

    m_waveWdg->AppendData(addr,weight);
    m_waveWdg->DisplayAllChannel(true);
}

void DialogWave::on_btnClear_clicked()
{
    m_waveWdg->Clear();
}

void DialogWave::on_btnClose_clicked()
{
    this->accept();
}

void DialogWave::on_btnSave_clicked()
{
    emit saveWave();
}
