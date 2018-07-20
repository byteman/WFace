#include "UartCfgWidget.h"
#include "ui_UartCfgWidget.h"

UartCfgWidget::UartCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UartCfgWidget)
{
    ui->setupUi(this);
}

UartCfgWidget::~UartCfgWidget()
{
    delete ui;
}
