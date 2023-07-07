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

