#ifndef TIMERWORKER_H
#define TIMERWORKER_H

#include <QObject>
#include <QMutex>
#include <QEvent>
class CmdHandler;
class TimerWorker : public QObject
{
    Q_OBJECT
public:
    explicit TimerWorker(QObject *parent = 0);
    void Start(int interval=0, CmdHandler* handler=NULL);
    void Stop();
signals:

public slots:
    void doWork();

private:
    CmdHandler* m_handler;
    int m_interval;
    bool m_stop;
    QMutex m_mutex;

};

#endif // TIMERWORKER_H
