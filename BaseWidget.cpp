#include "BaseWidget.h"

BaseWidget::BaseWidget(QWidget* parent):
    QWidget(parent)
{

}

BaseWidget::~BaseWidget()
{

}

MyConfig& BaseWidget::config()
{
    return cfg;
}

void BaseWidget::hidenForGuest()
{

}
#include <QLineEdit>
#include <QComboBox>
void BaseWidget::traversalControl(const QObjectList& q)
{
    foreach(QObject* obj,q)
    {
           QString class_name=obj->metaObject()->className();

           if(class_name=="QLineEdit")
           {
               QLineEdit* le=(QLineEdit*)obj;

               le->setText("");
           }
           else if(class_name=="QComboBox")
           {
               QComboBox* cbx=(QComboBox*)obj;

               cbx->setCurrentText("");
           }
    }

}
