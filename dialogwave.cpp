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
    this->show();
}

void DialogWave::onPollWeightResult(int addr, int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare)
{
    m_waveWdg->AppendData(addr,weight);
    m_waveWdg->DisplayAllChannel(true);
}
