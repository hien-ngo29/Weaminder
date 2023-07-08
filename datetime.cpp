#include "datetime.h"

DateTime::DateTime(QObject *parent)
    : QObject{parent}
{
    qDebug() << getCurrentRoundedTime();
}

QString DateTime::getCurrentTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("hh:mm");

    return timeString;
}

QList<QString> DateTime::getFollowingHours()
{
    QList<QString> resultList;

    for (int i = 0; i < 24; i++) {
        int hour = i;

        for (int j = 0; j < 2; j++) {
            resultList.append(QString::number(hour) + ( ( j == 0 ) ? ":00" : ":30" ));
        }
    }

    return resultList;
}

QString DateTime::getCurrentRoundedTime()
{
    QString currentTime = getCurrentTime();
    QString minuteSectionString = currentTime.split(":")[1];
    int minuteSection = minuteSectionString.toInt();

    minuteSectionString = (minuteSection < 15) ? "00" : "30";

    return currentTime.split(":")[0] + ":" + minuteSectionString;
}

