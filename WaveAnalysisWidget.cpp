#include "WaveAnalysisWidget.h"
#include "ui_WaveAnalysisWidget.h"
#include  "wavefile.h"
WaveAnalysisWidget::WaveAnalysisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveAnalysisWidget)
{
    ui->setupUi(this);
    waveWidget = new WaveWidget(ui->widget,1);
}

WaveAnalysisWidget::~WaveAnalysisWidget()
{
    delete ui;
}
int WaveAnalysisWidget::ReadWaveList()
{
    ui->listWave->clear();
    QStringList files;
    //devices->listWaveFiles(files);
    ui->listWave->addItems(files);
    return 0;
}

void WaveAnalysisWidget::on_listWave_itemClicked(QListWidgetItem *item)
{
    ChannelsData wvd;
    //devices->LoadWave(item->text(),wvd);
    waveWidget->SetData(wvd);
    waveWidget->DisplayAllChannel(true);
}

void WaveAnalysisWidget::on_btnClearWave_clicked()
{
    waveWidget->Clear();
}
