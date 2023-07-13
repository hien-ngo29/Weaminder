#include "jsonreader.h"

JsonReader::JsonReader(QObject *parent)
    : QObject{parent}
{

}

QJsonObject JsonReader::readJsonNetworkReply(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    return jsonResponse.object();
}

QJsonObject JsonReader::readJsonFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file:" << file.errorString();
        return {};
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qDebug() << "Error parsing JSON";
        return {};
    }

    QJsonObject jsonObj = jsonDoc.object();
    return jsonObj;
}

