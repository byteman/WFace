#include "customparams.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QLayout>
CustomParams::CustomParams():
    edtUI(NULL),
    cbxUI(NULL),
    parent(NULL)
{

}

bool CustomParams::update()
{
    if(type == "U8" || type=="I8"){
        value_size = 1;
    }else if(type == "U16" || type=="I16"){
        value_size = 2;
    }else if(type == "U32" || type=="I32" || type == "F32"){
        value_size = 4;
    }else if(type == "D64" ){
        value_size = 8;
    }else if(type[0] == 'B'){
            bool ok=false;
            value_size = type.mid(1).toInt(&ok);
            if(ok!=true)return false;

    }
    else{
        return false;
    }
    lblCaption = new QLabel(caption);
    parent->layout()->addWidget(lblCaption);
    if(className=="QLineEdit"){
        edtUI = new QLineEdit();
        parent->layout()->addWidget(edtUI);

    }else if(className=="QComboBox"){
        cbxUI = new QComboBox();
        parent->layout()->addWidget(cbxUI);
    }


    return true;
}



bool CustomParamsManager::load(QString path,QList<QGroupBox*> parents)
{
    QFile file(path);
    QByteArray byte_array;
    if(!file.exists()) return false;

    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    byte_array = file.readAll();
    file.close();

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
        return false;
    }
    if(parse_doucment.isObject())
    {
        QJsonObject obj = parse_doucment.object();
        qDebug() << obj.count();
        qDebug() << obj.keys();
        if(obj.contains("params"))
        {
            QJsonValue params = obj.take("params");

            if(params.isArray())
            {
               QJsonArray paraArray = params.toArray();
               QJsonValue value;
               foreach(value,paraArray){
                   QJsonObject o = value.toObject();
                   CustomParams p;
                   //p.attr = o.take("type");
                   p.caption = o.take("caption").toString();
                   p.className=o.take("class").toString();
                   p.reg_addr=o.take("reg_addr").toInt();
                   p.reg_num=o.take("reg_num").toInt();
                   p.type=o.take("type").toString();
                   p.parentName=o.take("parent").toString();
                   qDebug() << o.take("parent").toString();
                   qDebug() << o.take("label");
                   qDebug() << o.take("class");
                   qDebug() << o.take("type");
                   qDebug() << o.take("attr");
                   qDebug() << o.take("addr");
                   QGroupBox* parent;
                   foreach(parent,parents){
                       qDebug() << parent->objectName();
                       if(parent->objectName() == p.parentName)
                       {
                           qDebug() << "find";
                           p.parent = parent;
                           p.update();
                       }
                   }

               }



            }
        }
    }
   if(parse_doucment.isArray())
   {
           QJsonArray array = parse_doucment.array();
           int size = array.size();
           for(int i=0; i < size;i++)
            {
               QJsonValue value = array.at(i);
               if(value.isString())
               {
                    QString name = value.toString();
               }
               else if(value.isBool())
               {
                    bool flag = value.toBool();
               }
             }
   }
    return true;
}
