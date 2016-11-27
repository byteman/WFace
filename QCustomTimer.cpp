#include "qcustomtimer.h"  
  
//��̬�̳߳�Աָ���ʼ��  
QThread* QCustomTimer::m_pTimerThread = NULL;  
  
QCustomTimer::QCustomTimer(QObject *parent) :  
    QObject(parent)  
{
    if ( m_pTimerThread == NULL )  
    {  
        //���жϷ�֧������Qt����̬��Աָ�������ʼ��ΪNULL�����Ǻõ�����  
        //����������д�������ȫ�ֱ�����������������ϵ����ôӦ��Ҳ��ֻ��������  
        //��֤QCustomTimer�಻Ҫ������ȫ�ֱ�������������ϵ�������������ʼ������������  
        m_pTimerThread = new QThread;  
    }  
  
    m_pTimer = new QTimer(0);  
    m_pTimer->setSingleShot( true );//���δ���  
  
    m_pTimerThread->start();  
  
    m_pTimer->moveToThread( m_pTimerThread );//���Ķ�ʱ�����е��߳�  
  
    connect( m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()) , Qt::DirectConnection );//��ʱ���¼�������  
  
    connect( this, SIGNAL(startSignal(int)), m_pTimer, SLOT(start( int ) ), Qt::BlockingQueuedConnection );//���Ӷ�ʱ�������ۺ����������á�ֱ����  
  
    connect( this, SIGNAL(stopSignal()), m_pTimer, SLOT(stop()), Qt::BlockingQueuedConnection );//���Ӷ�ʱ���رղۺ����������á�ֱ����  
  
    connect( this, SIGNAL( deletelater() ), m_pTimer, SLOT(deleteLater()) );//ɾ��λ���߳��еĶ�ʱ�����󣬲���һ����ʱɾ�����¼�  
}  
  
QCustomTimer::~QCustomTimer()  
{  
    StopTimer();  
    DeleteLater();  
}  
  
void QCustomTimer::onTimer()  
{  
    emit TimeOut();//���䶨ʱ�������ź�  
}  
  
void QCustomTimer::StartTimer(int nMsc)  
{  
    emit startSignal(nMsc) ;//�����߳��ڵĶ�ʱ�����Ϳ�����ʱ���ź�  
}  
  
void QCustomTimer::StopTimer()  
{  
    emit stopSignal();//�����߳��ڵĶ�ʱ������ֹͣ��ʱ���ź�  
}  
  
void QCustomTimer::DeleteLater()  
{  
    emit deletelater();//�����̵߳��¼�ѭ������һ������ɾ���¼�  
}  
