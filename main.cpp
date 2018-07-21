#include "mainwindow2.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QSplashScreen>
#include <QTextCodec>
void loadLang(QTranslator& translator)
{

    QSettings config("wface.ini", QSettings::IniFormat);
//注意在程序中必须都设置为一样的编码方式，否则还是会出现乱码.
    config.setIniCodec("utf-8");
    QString lang       = config.value("/config/lang","zh").toString();
    if(lang == "zh")
    {
        translator.load(":/cn.qm");
    }
    else
    {
        translator.load(":/en.qm");
    }
}
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    QPixmap pixmap("splash.jpg");
//    QSplashScreen splash(pixmap);
//    splash.show();
    QTranslator translator;
    loadLang(translator);
    a.installTranslator(&translator);

    /*使程序在显示启动画面的同时仍能响应鼠标等其他事件*/
    a.processEvents();

    MainWindow w(a);
    w.show();
    //splash.finish(&w);
    return a.exec();
}
