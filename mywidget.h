#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QWidget>
class MyWidget : public QFrame
{
public:
    MyWidget(int addr,QWidget* parent);

    void Display(int weight, quint16 state,quint16 dot);
    void DisplayWeight(int addr, int weight, quint16 state, quint16 dot);
private:
    int m_addr;
    QLabel* m_weight;
    QWidget* m_state;
    QLabel* m_dev_addr;
    QLabel* m_zero_state;
    QLabel* m_still_state;
    QLabel* m_ng_state;

    void clearState();
};

#endif // MYWIDGET_H
