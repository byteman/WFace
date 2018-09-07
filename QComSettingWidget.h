#ifndef QCOMSETTINGWIDGET_H
#define QCOMSETTINGWIDGET_H

#include <QWidget>
#include "ModbusReader.h"
namespace Ui {
class QComSettingWidget;
}

class QComSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QComSettingWidget(QWidget *parent = 0,QString port="");
    ~QComSettingWidget();
    void SetComPort(int channel,int currentPort);
    ModbusReader* GetRtuReader();
signals:
    void onPortStateChange(int );
private slots:
    void on_btnComOpera_clicked();

private:
    int m_channel;
    int m_current_port;
    QString m_port;
    ModbusReader *m_reader;
    Ui::QComSettingWidget *ui;
};

#endif // QCOMSETTINGWIDGET_H
