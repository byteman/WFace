#ifndef DIALOGWAVE_H
#define DIALOGWAVE_H

#include <QDialog>
#include "wavewidget.h"
namespace Ui {
class DialogWave;
}

class DialogWave : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWave(QWidget *parent = 0,int num=1);
    ~DialogWave();
    void SetChannel(int start,int num);
public slots:
    void onPollWeightResult(int addr,int weight, quint16 state,quint16 dot, qint32 gross,qint32 tare);
private:
    Ui::DialogWave *ui;
    WaveWidget *m_waveWdg;
    int m_channel;
};

#endif // DIALOGWAVE_H
