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
};

#endif // ADC102_H
