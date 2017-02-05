#ifndef CUSTOMPARAMS_H
#define CUSTOMPARAMS_H

#include <QComboBox>
#include <QLineEdit>
#include <QVariant>
#include <QList>
#include <QGroupBox>
#include <QLabel>
class CustomParams
{
public:
    CustomParams();
    bool update();
    QString className;
    QString caption;
    QString type;
    QString parentName;
    QVariant value;

    int value_size;
    quint8 attr; //0:R 1:W 2:RW
    int reg_addr;
    int reg_num;
    QLineEdit* edtUI;
    QComboBox* cbxUI;
    QLabel* lblCaption;
    QGroupBox* parent;
};

class CustomParamsManager
{
public:
    CustomParamsManager(){}
    bool load(QString path,QList<QGroupBox*> parents);
private:
    QList<CustomParams> paramList;
};
#endif // CUSTOMPARAMS_H
