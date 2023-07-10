#include "tasksloader.h"

TasksLoader::TasksLoader(QObject *parent)
    : QObject{parent}
{

}

QList<QString> TasksLoader::getSuitableTasksWithWeather()
{
    QList<QString> tasksToDo;

    if (temperature() >= 30)
        tasksToDo.append("Bring your fan");
    if (weatherStatus().contains("rain")) {
        tasksToDo.append("Bring your umbrella");
        tasksToDo.append("Bring your raincoat");
    }
    if (temperature() <= 25)
        tasksToDo.append("Wear your sweater");

    return tasksToDo;
}

QList<QString> TasksLoader::tasksList() const
{
    return m_tasksList;
}

void TasksLoader::setTasksList(QList<QString> newTasksList)
{
    if (m_tasksList == newTasksList)
        return;
    m_tasksList = newTasksList;
    emit tasksListChanged();
}

int TasksLoader::temperature() const
{
    return m_temperature;
}

void TasksLoader::setTemperature(int newTemperature)
{
    if (m_temperature == newTemperature)
        return;
    m_temperature = newTemperature;
    emit temperatureChanged();
}

QString TasksLoader::weatherStatus() const
{
    return m_weatherStatus;
}

void TasksLoader::setWeatherStatus(const QString &newWeatherStatus)
{
    if (m_weatherStatus == newWeatherStatus)
        return;
    m_weatherStatus = newWeatherStatus;
    emit weatherStatusChanged();
}

