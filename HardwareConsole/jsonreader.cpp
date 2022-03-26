#include "jsonreader.h"
#include <QFile>
#include <QDebug>
#include <unistd.h>

JsonReader::JsonReader(QObject *parent)
    : QObject{parent}
{
}

bool JsonReader::writeJsonFile(const QJsonObject & object) {
    QFile file(m_source);
    bool ok = file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    if (!ok) {
        qDebug() << QString("failed to open file: %1").arg(__FILE__);
        return false;
    }
    QJsonDocument jsonDocument;
    jsonDocument.setObject(object);
    QByteArray byteArray = jsonDocument.toJson(QJsonDocument::Indented);
    QTextStream out(& file);
    out.setCodec("UTF-8");
    out << byteArray;
    if(file.isOpen())
        file.close();
    return true;
}

QJsonObject JsonReader::readJsonFile()
{
    QFile file(m_source);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open json file failed!";
        return QJsonObject();
    }
    QByteArray allData = file.readAll();
    if(allData.isEmpty())
        return QJsonObject();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, & json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "JsonReader::readJsonFile()" << "json error!";
        return QJsonObject();
    }
    QJsonObject rootObj = jsonDoc.object();
    if(file.isOpen())
        file.close();
    return rootObj;
}

void JsonReader::clearJsonObject(QJsonObject & object)
{
  QStringList strList = object.keys();
   for(int i = 0; i < strList.size(); ++i)
    {
      object.remove(strList.at(i));
    }
}
