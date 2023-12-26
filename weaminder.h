#ifndef WEAMINDER_H
#define WEAMINDER_H

#include <QDebug>
#include <QObject>
#include <QList>

#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

class Weaminder : public QObject
{
    Q_OBJECT
public:
    explicit Weaminder(QObject *parent = nullptr);

    Q_INVOKABLE bool isConnectionOn();

signals:

};

#endif // WEAMINDER_H
