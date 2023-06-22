#include "datetime.h"

DateTime::DateTime(QObject *parent)
    : QObject{parent}
{
    dateFormat = "dd/mm/yy";
    qDebug() << getCurrentDayInWeek();
}

QString DateTime::dateToDateFormat(int day, int month, int year)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << day << '/'
       << std::setfill('0') << std::setw(2) << month << '/'
       << std::setw(2) << (year % 100);

    QString formattedDate = QString::fromStdString(ss.str());

    if (dateFormat == "dd/mm/yyyy") {
        return formattedDate;
    } else if (dateFormat == "mm/dd/yyyy") {
        return formattedDate.mid(3, 2) + '/' + formattedDate.mid(0, 2) + '/' + formattedDate.mid(6, 2);
    } else if (dateFormat == "yyyy/mm/dd") {
        return "20" + formattedDate.mid(6, 2) + '/' + formattedDate.mid(3, 2) + '/' + formattedDate.mid(0, 2);
    } else if (dateFormat == "dd/mm/yy") {
        return formattedDate.left(8);
    } else {
        return "Invalid date format!";
    }
}

QString DateTime::getCurrentTime()
{
    std::time_t currentTime = std::time(nullptr);
    std::string stdStringTime = std::ctime(&currentTime);
    stdStringTime = stdStringTime.substr(11, 8); // Extract only the time part

    QString timeString = QString::fromUtf8(stdStringTime.c_str());

    return timeString;
}

QString DateTime::getCurrentDate()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* currentTm = std::localtime(&currentTime);

    int year = currentTm->tm_year + 1900; // Years since 1900
    int month = currentTm->tm_mon + 1;    // Months since January (0-11)
    int day = currentTm->tm_mday;       // Day of the month (1-31)

    return dateToDateFormat(day, month, year);
}

QString DateTime::getCurrentDayInWeek()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* currentTm = std::localtime(&currentTime);

    int currentDayOfWeekIndex = currentTm->tm_wday; // Sunday = 0, Monday = 1, ..., Saturday = 6

    const char* dayNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    return dayNames[currentDayOfWeekIndex];
}
