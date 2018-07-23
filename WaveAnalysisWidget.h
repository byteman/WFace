#ifndef WAVEANALYSISWIDGET_H
#define WAVEANALYSISWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "wavewidget.h"
namespace Ui {
class WaveAnalysisWidget;
}

class WaveAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveAnalysisWidget(QWidget *parent = 0);
    ~WaveAnalysisWidget();

private slots:
    void on_listWave_itemClicked(QListWidgetItem *item);

    void on_btnClearWave_clicked();

private:
    Ui::WaveAnalysisWidget *ui;
    int ReadWaveList();
    WaveWidget*  waveWidget;
};

#endif // WAVEANALYSISWIDGET_H
