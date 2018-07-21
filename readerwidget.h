#ifndef READERWIDGET
#define READERWIDGET
#include "RtuReader.h"
#include <QList>
#include <QWidget>
class IReaderWidget:public QWidget{
    Q_OBJECT
public:
    IReaderWidget(QWidget* parent);
    virtual RtuReader* GetReader(void);
    virtual QList<RtuReader*> GetReaderList(void);
protected:
    RtuReader        m_reader;
    QList<RtuReader*> m_readers;
};
#endif // READERWIDGET


