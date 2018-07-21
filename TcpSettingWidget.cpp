#include "TcpSettingWidget.h"
#include "ui_TcpSettingWidget.h"

TcpSettingWidget::TcpSettingWidget(QWidget *parent) :
    IReaderWidget(parent),
    ui(new Ui::TcpSettingWidget)
{
    ui->setupUi(this);
}

TcpSettingWidget::~TcpSettingWidget()
{
    delete ui;
}
