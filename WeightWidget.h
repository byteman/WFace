#ifndef WEIGHTWIDGET_H
#define WEIGHTWIDGET_H

#include <QWidget>
#include "weighthandler.h"
#include "BaseWidget.h"
namespace Ui {
class WeightWidget;
}

class WeightWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit WeightWidget(QWidget *parent = 0);
    ~WeightWidget();

protected slots:
    void onWeightResult(int weight, quint16 state, quint16 dot, qint32 gross, qint32 tare);
    void onWeightParaRead(quint16 div_high, quint16 div_low, quint32 full_high, quint32 full_low, int dot);
private slots:
    void on_btnGN_clicked();

    void on_btnTare_clicked();

    void on_btnZoom10_clicked();

    void on_btnZero_clicked();

private:
    Ui::WeightWidget *ui;
    void InitUI();
    RtuReader       *reader;
    WeightHandler   *weight;
    void clearState();
};

#endif // WEIGHTWIDGET_H
