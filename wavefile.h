#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <QFile>
#include <QVector>
#include <QMap>
typedef QVector<double> ChannelSample;
typedef QMap<int,ChannelSample > ChannelsData;

class WaveFile
{
public:
    WaveFile();
    ~WaveFile();
    WaveFile(QString file);
    bool load(QString file,ChannelsData& samples);
    qint64 write(QByteArray& data);
    qint64 writeChan(int addr,QByteArray& data);
    void close();
private:
    QFile *m_file;
};

#endif // WAVEFILE_H
