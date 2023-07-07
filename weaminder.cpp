#include "weaminder.h"

Weaminder::Weaminder(QObject *parent)
    : QObject{parent}
{

}

QList<QString> Weaminder::getFollowingHours()
{
    QList<QString> result;

    for (int i = 0; i < 24; i++) {
        result.append(QString::number(i) + ":00");
    }

    return result;
}
