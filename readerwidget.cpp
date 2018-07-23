
#include "readerwidget.h"

IReaderWidget::IReaderWidget(QWidget *parent):
    QWidget(parent),
    m_scaner(NULL)
{
    m_scaner = new ScanHandler(&m_reader);
}


RtuReader *IReaderWidget::GetReader()
{

    return &m_reader;
}

QList<RtuReader *> IReaderWidget::GetReaderList()
{

    return m_readers;

}
