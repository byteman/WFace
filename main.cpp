#include "mainwindow2.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QSplashScreen>
#include "Logger.h"
#include "ConsoleAppender.h"
#include "FileAppender.h"
#include "command.h"
#include "PCOMM.H"
void loadLang(QTranslator& translator)
{

    QSettings config("wface.ini", QSettings::IniFormat);

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
void uart_test()
{
    char buf[1024];
    if(SIO_OK == sio_open(1))
    {
        sio_SetReadTimeouts(1,3000,10);
        int ret = sio_read(1,buf,1024);
        LOG_ERROR("ret = %d",ret);
    }
}
int main(int argc, char *argv[])
{



    ConsoleAppender* consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    cuteLogger->registerAppender(consoleAppender);
    cuteLogger->registerAppender(new FileAppender("wface.log"));
    LOG_DEBUG("app start %s",__DATE__);


    QApplication a(argc, argv);
//    QPixmap pixmap("splash.jpg");
//    QSplashScreen splash(pixmap);
//    splash.show();
    QTranslator translator;
    loadLang(translator);
    a.installTranslator(&translator);

    /*使程序在显示启动画面的同时仍能响应鼠标等其他事件*/
    a.processEvents();
    //加载样式表
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    a.setFont(QFont("Microsoft Yahei", 9));
    AppInit::Instance()->start();
    MainWindow w(a);
    w.show();
    //splash.finish(&w);
    return a.exec();
}
