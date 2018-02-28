#ifndef UTILS_H
#define UTILS_H

#include <QObject>
class utils
{
public:
    utils();
    static QString formatfloat(float wf, int dot);
    static QString float2string(float wf, int dot);
};

#endif // UTILS_H
