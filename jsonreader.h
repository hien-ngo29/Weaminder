#ifndef JSONREADER_H
#define JSONREADER_H

#include <QObject>

#include <QFile>

#include <QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

class JsonReader : public QObject
{
    Q_OBJECT
public:
    explicit JsonReader(QObject *parent = nullptr);

    QJsonObject readJsonNetworkReply(QNetworkReply *reply);

    QJsonObject readJsonFile(QString path);

signals:

};

#endif // JSONREADER_H
