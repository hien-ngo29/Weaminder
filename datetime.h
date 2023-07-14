#ifndef DATETIME_H
#define DATETIME_H

#include <QObject>
#include <QDebug>

#include <iostream>
#include <QDateTime>
#include <QDebug>

class DateTime : public QObject
{
    Q_OBJECT
public:
    explicit DateTime(QObject *parent = nullptr);

    Q_INVOKABLE QString getCurrentTime();

    Q_INVOKABLE QList<QString> getFollowingHours();

    Q_INVOKABLE QString getCurrentRoundedTime();
    // "rounded time" means round it to 1(:00)

signals:

private:

};

#endif // DATETIME_H
