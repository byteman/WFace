#ifndef MODBUSREADERFACTORY_H
#define MODBUSREADERFACTORY_H

#include "ModbusReader.h"
#include "SingleModbusReader.h"
#include "MultiModbusReader.h"
#include <QVector>
#include "ModbusTcp.h"
#include "ModbusRtu.h"
class ModbusReaderFactory
{
public:
    ModbusReaderFactory();
    void start(int ms);
    void stop();
    ModbusRtu* GetRtuChannel(int index);
    ModbusTcp* GetTcpChannel();
    //int GetRtuNr();
    QStringList& GetRtuNr();
    ModbusReader*  GetReader(QString type);
    SingleModbusReader*  GetSingleReader();
    MultiModbusReader*   GetMultiReader();
private:
    SingleModbusReader*  m_single_reader;
    MultiModbusReader*   m_multi_reader;
    ModbusReader*  m_current_reader;
    ModbusTcp* m_tcp;
    ModbusRtu* m_rtu;
    QVector<ModbusRtu*> m_rtu_list;
    QStringList m_com_list;

};

#endif // MODBUSREADERFACTORY_H
