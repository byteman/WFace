#include "utils.h"

utils::utils()
{

}

#include <QTextCodec>

int utils::toInt(QString txt,int dot,bool *ok)
{
    float tmp = txt.toFloat(ok);
    if(!ok) return false;


    return tmp*pow(10,dot);

}

QString utils::format2(int value, int dot)
{

    QString svalue = QString("%1").arg(value);
    if((svalue.size() > dot) )
    {
        if(dot > 0)
        {
            int index = svalue.size() - dot;
            svalue.insert(index,".");
        }

    }
    else
    {
        int n =  dot - svalue.size();
        QString s = QString("0.%1").arg(QString().fill('0',n));
        svalue.prepend(s);
    }
    return svalue;
}

QString utils::formatValue(int value, int dot)
{
    QString str;
    str = format2(value,dot);

    return str;
}
QString utils::GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
    return g2u;
}

QString utils::UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    return utf2gbk;
}

std::string utils::gbk2utf8(const QString &inStr)
{
    return GBK2UTF8(inStr).toStdString();
}

QString utils::utf82gbk(const std::string &inStr)
{
    QString str = QString::fromStdString(inStr);

    return UTF82GBK(str);
}
float utils::int2float(int wt, int dot)
{
    float wf = (float)wt;
    switch(dot)
    {

        case 1:
            return wf/10.0f;
            break;
        case 2:
            return wf/100.0f;
            break;
        case 3:
            return wf/1000.0f;
            break;
        case 4:
            return wf/10000.0f;
            break;
        default:
            return wf;
            break;
    }
    return wf;
}
QString utils::formatfloat(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}
QString utils::float2string(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf/10);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf/100);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf/1000);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf/10000);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}
