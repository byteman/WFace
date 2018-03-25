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
    bool Append(int addr,QString value,int maxAddr);
private:
    int m_index; //存储序号.
    QFile m_file;
    QString CreateDir();
    QString GetFileName();
    QStringList m_values;
    QMap<int,QString >    m_csv_values;

};

#endif // CSVFILE_H
