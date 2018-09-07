#include "ModbusReaderFactory.h"
#include "SingleModbusReader.h"
#include "MultiModbusReader.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
ModbusReaderFactory::ModbusReaderFactory():
    m_single_reader(NULL),
    m_multi_reader(NULL)
{
    m_single_reader = new SingleModbusReader();
    m_multi_reader  = new MultiModbusReader();
    m_tcp = new ModbusTcp();
    m_rtu = new ModbusRtu();

    QSettings config("wface.ini", QSettings::IniFormat);
//注意在程序中必须都设置为一样的编码方式，否则还是会出现乱码.
    config.setIniCodec("utf-8");

    bool isAuto = config.value("/uart/auto",true).toBool();

    QString coms = config.value("/uart/com","").toString();
    QStringList comList;

    if(isAuto){
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

        QSerialPortInfo port;
        foreach(port,ports){
            m_com_list.push_back(port.portName());
        }
    }else{
        //手工方式，读取配置文件中的串口个数.
        comList = coms.split(",");
        for(int j = 0; j < comList.size(); j++)
        {
            //int index =comList.at(j).toInt();
            m_com_list.push_back(comList.at(j));
        }
    }

    int nr = m_com_list.size();
    if(nr <=0) nr = 1; //至少创建一个.
    //根据配置文件中串口的个数预先创建RTU类.
    for(int i = 0; i < nr; i++)
    {
        m_rtu_list.push_back(new ModbusRtu(i));
    }
}

void ModbusReaderFactory::stop()
{

}

ModbusRtu *ModbusReaderFactory::GetRtuChannel(int index)
{
    if(index >= m_rtu_list.size() || index < 0){
        return NULL;
    }
    return m_rtu_list[index];
}

ModbusTcp *ModbusReaderFactory::GetTcpChannel()
{
    return m_tcp;
}

QStringList& ModbusReaderFactory::GetRtuNr()
{
    return m_com_list;
}


void ModbusReaderFactory::start(int ms)
{
    if(m_single_reader){
        m_single_reader->start(ms);
    }
    if(m_multi_reader){
        m_multi_reader->start(ms);
    }

}

ModbusReader *ModbusReaderFactory::GetReader(QString type)
{
   if(type == "multi"){
       return m_multi_reader;
   }else{
       return m_single_reader;
   }
}

SingleModbusReader *ModbusReaderFactory::GetSingleReader()
{
    return     m_single_reader;
}

MultiModbusReader *ModbusReaderFactory::GetMultiReader()
{
    return     m_multi_reader;
}

