#include "weaminder.h"

Weaminder::Weaminder(QObject *parent)
    : QObject{parent}
{

}

QList<QString> Weaminder::getFollowingHours()
{
    QList<QString> result;

    for (int i = 0; i < 24; i++) {
        int hour = i;

        for (int j = 0; j < 2; j++) {
            result.append(QString::number(hour) + ( ( j == 0 ) ? ":00" : ":30" ));
        }
    }

    return result;
}
