#ifndef ADC102_H
#define ADC102_H

#include <QObject>
#include "mymodbus.h"
#include <QTimer>
#include "cmdhandler.h"
#include "parahandler.h"
#include <QList>
#include "updatehandler.h"
#include "timerworker.h"
#include "RtuReader.h"

enum ADC102_REGS{

    REG_DIV_HIGH=8,
    REG_CALIB=20,
    REG_FULL_SPAN=26,

    REG_ADDR=30,
    REG_4B_CORN_K=36, //40037/40038 传感器1号角差系数(1000代表1.000)
    REG_2B_AUTO_CORN=44, //自动角差控制：0：启动标定；1：标定传感器1；2：标定传感器2；3：标定传感器3；4：标定传感器4；5：结束标定
    REG_2B_SENSOR_NUM=46,//传感器个数
    REG_4B_CHANNEL_AD=49, //第一路通道的AD值 整数值


};

#endif // ADC102_H
