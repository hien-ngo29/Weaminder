#include "datetime.h"

DateTime::DateTime(QObject *parent)
    : QObject{parent}
{

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
        resultList.append(QString::number(i) + ":00");
    }

    return resultList;
}

QString DateTime::getCurrentRoundedTime()
{
    QString currentTime = getCurrentTime();
    QString hourSectionString = currentTime.split(":")[0];
    QString minuteSectionString = currentTime.split(":")[1];
    int hourSection = hourSectionString.toInt();
    int minuteSection = minuteSectionString.toInt();

    minuteSectionString = "00";
    if (minuteSection > 30) {
        hourSection += 1;

        QString newHourString = QString::number(hourSection);
        hourSectionString = (newHourString.length() < 2 ? "0" : "") + newHourString;
    }

    return hourSectionString + ":" + minuteSectionString;
}

QStringList DateTime::getFollowingDaysInWeek()
{
    return {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
}

QStringList DateTime::getFollowingDaysInWeekFromToday()
{
    QStringList resultList = getFollowingDaysInWeek();
    QDate currentDate = QDate::currentDate();
    for (int i = 0; i < currentDate.dayOfWeek()-1; i++) {
        resultList.insert(6, resultList.takeAt(0));
    }
    return resultList;
}

