
#include "readerwidget.h"

IReaderWidget::IReaderWidget(QWidget *parent):
    QWidget(parent)
{

}

RtuReader *IReaderWidget::GetReader()
{

    return &m_reader;
}

QList<RtuReader *> IReaderWidget::GetReaderList()
{

    return m_readers;

}
