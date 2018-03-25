#ifndef UTILS_H
#define UTILS_H

#include <QObject>
class utils
{
public:
    utils();
    static QString formatfloat(float wf, int dot);
    static QString float2string(float wf, int dot);
    static float int2float(int wt, int dot);
    static QString GBK2UTF8(const QString &inStr);
    static QString UTF82GBK(const QString &inStr);
    static std::string gbk2utf8(const QString &inStr);
    static QString utf82gbk(const std::string &inStr);
};

#endif // UTILS_H
