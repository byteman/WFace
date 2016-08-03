#include "mainwindow2.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    QTranslator translator;
    bool b = false;
    //QString path = QCoreApplication::applicationDirPath() + "/cn.qm";
    b = translator.load(":/cn.qm");
    a.installTranslator(&translator);

    MainWindow w(a);
    w.show();

    return a.exec();
}
