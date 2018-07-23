#ifndef READERWIDGET
#define READERWIDGET
#include "RtuReader.h"
#include <QList>
#include <QWidget>
#include "scanhandler.h"

//Connector -> reader + scaner
class IReaderWidget:public QWidget{
    Q_OBJECT
public:
    IReaderWidget(QWidget* parent);
    //void SetHandler(ScanHandler* handler);
    virtual RtuReader* GetReader(void);
    virtual QList<RtuReader*> GetReaderList(void);
signals:
    void StartSearch(IReaderWidget* reader,bool start);
protected:
    RtuReader           m_reader;
    QList<RtuReader*>   m_readers;
    ScanHandler         *m_scaner;
};
#endif // READERWIDGET


