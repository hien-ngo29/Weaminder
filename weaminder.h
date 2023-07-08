#ifndef WEAMINDER_H
#define WEAMINDER_H

#include <QDebug>
#include <QObject>
#include <QList>

class Weaminder : public QObject
{
    Q_OBJECT
public:
    explicit Weaminder(QObject *parent = nullptr);

signals:

};

#endif // WEAMINDER_H
