#include "csvfile.h"
#include <QDir>
#include <QTextStream>
#include "utils.h"
QString CSVFile::CreateDir()
{

    QString target_dir=QString("%1/csv/").arg(QDir::currentPath());
    QDir dir(target_dir);
    if(!dir.exists())
    {
        dir.mkpath(target_dir);
    }
    return target_dir;
}
QString CSVFile::GetFileName()
{
    QString dt  = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString dir = CreateDir();
    QString file = QString("%1/%2.csv").arg(dir).arg(dt);
    return file;
}
CSVFile::CSVFile():
    m_index(0)
{
    m_file.setFileName(GetFileName());
    m_file.open(QIODevice::WriteOnly|QIODevice::Append);

    m_values.push_back(tr("index"));
    m_values.push_back(tr("time"));
    for(int i=0; i < 32; i++)
    {
        m_values.push_back(tr("address") + QString("%1").arg(i+1));
    }
    m_values.push_back("\r\n");

    QString out = m_values.join(',');
    m_file.write(out.toUtf8());
    m_file.flush();

    for(int i=2; i < 34; i++)
    {
        m_values[i] = "";
    }

}

bool CSVFile::Append(QMap<int, QString> &values, int maxAddr)
{
    QMapIterator<int, QString> i(values);
    while (i.hasNext()) {
        i.next();
        m_values[i.key()] = i.value();

    }
    m_values[0] = QString("%1").arg(m_index++);
    m_values[1] = QDateTime::currentDateTime().toString("%yyyy-MM-dd hh-mm-ss");

    m_file.write(m_values.join(',').toLatin1());
    return true;
}

bool CSVFile::Append(int addr, QString value, int maxAddr)
{

    m_values[addr+1] = value;
    if(addr >= maxAddr)
    {
        m_values[0] = QString("%1").arg(m_index++);
        m_values[1] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString out = m_values.join(',');

        //QTextStream qts(&m_file);
        //qts << out << "\r\n";

        m_file.write(out.toUtf8());
        if( (m_index % 10) == 0)
        {
            m_file.flush();
        }
        return true;
    }
    return false;
}



