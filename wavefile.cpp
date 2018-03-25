#include "wavefile.h"

WaveFile::WaveFile():
    m_file(NULL)
{


}

WaveFile::~WaveFile()
{
    if(m_file!=NULL)
    {
        m_file->close();
         delete m_file;
    }

}

WaveFile::WaveFile(QString file)
{
    m_file = new QFile(file);
    m_file->open(QIODevice::ReadWrite|QIODevice::Append);
}

bool WaveFile::load(QString file, ChannelsData &samples)
{
    if(m_file!=NULL)
    {
        delete m_file;
        m_file = NULL;
    }
    m_file = new QFile(file);
    if(!m_file->open(QIODevice::ReadOnly))
    {
        return true;
    }

    QByteArray data = m_file->readAll();

    int addr = 0;
    int size = 0;
    float value;
    bool ok = false;

    QByteArray &temp = data;
    do{
        ChannelSample sample;

        memcpy(&addr, temp.data(),sizeof(int));
        temp = temp.remove(0,sizeof(int));
        memcpy(&size, temp.data(),sizeof(int));
        temp = temp.remove(0,sizeof(int));

        for(int i = 0; i < size; i++)
        {
            memcpy(&value, temp.data(),sizeof(float));
            temp = temp.remove(0,sizeof(float));
            sample.push_back(value);
        }

        samples[addr] = sample;

    }while(temp.size());
    return true;

}
// sample1: 1->2bytes 2->2bytes 3->bytes
// sample2: 1->2bytes 2->2bytes 3->bytes
// sample3: 1->2bytes 2->2bytes 3->bytes
qint64 WaveFile::write(QByteArray &data)
{
    if(m_file==NULL) return -1;
    return m_file->write(data);
}

qint64 WaveFile::writeChan(int addr, QByteArray &data)
{
    if(m_file==NULL) return -1;
    int size = data.size() / 4;

    m_file->write((const char*)&addr,sizeof(int));
    m_file->write((const char*)&size,sizeof(int));


    return m_file->write(data);
}

void WaveFile::close()
{
    if(m_file==NULL)return;
    m_file->close();
}

