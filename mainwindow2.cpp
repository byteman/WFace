#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "QFile"
MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);
    QFile file(":/mystyle.txt");
    file.open(QFile::ReadOnly);


    QByteArray res = file.readAll();


    this->setStyleSheet(res);
}

MainWindow2::~MainWindow2()
{
    delete ui;
}
