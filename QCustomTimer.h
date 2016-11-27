#ifndef QCUSTOMTIMER_H  
#define QCUSTOMTIMER_H  
  
#include <QObject>  
#include <QTimer>  
#include <QThread>  
  
class QCustomTimer : public QObject  
{  
    Q_OBJECT  
public:  
    explicit QCustomTimer(QObject *parent = 0);  
    ~QCustomTimer();  
  
private:  
    static QThread     *m_pTimerThread; //��ʱ�������߳�  
    QTimer             *m_pTimer;       //��ʱ������  
  
signals:  
    void startSignal( int nMsc );//������ʱ���ź�  
    void stopSignal();           //ֹͣ��ʱ���ź�  
    void TimeOut();              //��ʱ���������ⲿ�����Ӵ��ź�  
    void deletelater();          //��ʱɾ����ʱ���ź�  
public slots:  
    void onTimer();              //�����ڲ���ʱ�����ۺ��������ⲿ���䶨ʱ�������ź�  
public:  
    void StartTimer( int nMsc ); //������ʱ��  
    void StopTimer();            //�رն�ʱ��  
    void DeleteLater();          //��ʱɾ����ʱ������  
  
};  
  
#endif // QCUSTOMTIMER_H  