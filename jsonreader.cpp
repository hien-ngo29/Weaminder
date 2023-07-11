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
    QString rawContent;
    QFile file;

    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    rawContent = file.readAll();
    file.close();

    qWarning() << rawContent;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawContent.toUtf8());
    return jsonDoc.object();
}

