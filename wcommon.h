#ifndef WCOMMON_H
#define WCOMMON_H

#include <qglobal.h>
#define REG_RT_WT        5
#define REG_SENOSR_NUM   8
#define REG_CALC_TYPE    35
#define REG_CALC_ZERO    37
#define REG_SENSOR_K     38
#define REG_SENSOR_Z     62
#define REG_SENSOR_WT    86
#define REG_SENSOR_MV    110
#define REG_INPUT_WTS    134
#define REG_CALC_K       158
#define REG_K       12
typedef union _WFloatType
{
    float value;
    quint16 buffer[2];
}WFloatType;

typedef union _WInt32Type
{
    qint32 value;
    quint16 buffer[2];
}WIntType;

#endif // WCOMMON_H

