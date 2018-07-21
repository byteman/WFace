#include "UartSettingWidget.h"
#include "ui_UartSettingWidget.h"

UartSettingWidget::UartSettingWidget(QWidget *parent) :
    IReaderWidget(parent),
    ui(new Ui::UartCfgWidget)
{
    ui->setupUi(this);
}

UartSettingWidget::~UartSettingWidget()
{
    delete ui;
}

void UartSettingWidget::on_btnSearch_clicked()
{

}
