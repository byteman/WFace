#ifndef CSVFILE_H
#define CSVFILE_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QDateTime>
class CSVFile:public QObject
{
    Q_OBJECT
public:
    CSVFile();
    //存储采集一圈的数据.
    bool Append(QMap<int,QString> &values,int maxAddr);
    bool Append(int addr,QString value,int maxAddr,bool flush=false);
    bool SaveFile(QString name);
private:
    int m_index; //存储序号.
    QFile m_file;
    QString CreateDir();
    QString GetFileName();
    QStringList m_values;
    QByteArray  m_output;
    QByteArray  m_header;
    QMap<int,QString >    m_csv_values;

    bool writeHeader(QString name);
};

#endif // CSVFILE_H
