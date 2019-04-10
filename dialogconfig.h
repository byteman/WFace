#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>
#include "myconfig.h"
namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfig(QWidget *parent = 0);
    ~DialogConfig();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_groupBox_4_clicked();

    void on_pushButton_clicked();

    void on_chkParam_clicked();

private:
    Ui::DialogConfig *ui;
    MyConfig config;
};

#endif // DIALOGCONFIG_H
