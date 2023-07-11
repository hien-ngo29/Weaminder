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

