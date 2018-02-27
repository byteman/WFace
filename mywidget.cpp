#include "mywidget.h"
#include <QHBoxLayout>
MyWidget::MyWidget(QObject *parent):
    QWidget(parent),
    m_addr(0)
{
    m_weight = new QLabel(this);
    m_zero_state = new QLabel(this);
    m_still_state = new QLabel(this);
    m_ng_state = new QLabel(this);
    m_state = new QWidget(this);
    QVBoxLayout *item_layout  = new QVBoxLayout;


    QHBoxLayout *state_layout = new QHBoxLayout;
    state_layout->addWidget(m_zero_state);
    state_layout->addWidget(m_still_state);
    state_layout->addWidget(m_ng_state);
    m_state->setLayout(state_layout);


    item_layout->addItem(m_weight);
    item_layout->addItem(state_layout);

    this->setLayout(item_layout);
}



void MyWidget::Display(int weight, quint16 state, quint16 dot)
{

}
