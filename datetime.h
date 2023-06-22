#ifndef DATETIME_H
#define DATETIME_H

#include <QObject>
#include <QDebug>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>

class DateTime : public QObject
{
    Q_OBJECT
public:
    explicit DateTime(QObject *parent = nullptr);

    QString dateToDateFormat(int day, int month, int year);

    Q_INVOKABLE QString getCurrentTime();
    Q_INVOKABLE QString getCurrentDate();
    Q_INVOKABLE QString getCurrentDayInWeek();

signals:

private:
    QString dateFormat;

    int m_day;
    int m_month;
    int m_year;

    int m_hour;
    int m_minute;
    int m_second;
};

#endif // DATETIME_H
