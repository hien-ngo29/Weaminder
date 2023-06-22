#include "tasksloader.h"

TasksLoader::TasksLoader(QObject *parent)
    : QObject{parent}
{

}

QList<QString> TasksLoader::getSuitableTasksWithWeather()
{
    QList<QString> tasksToDo;

    double temperature = currentWeather()->temperature();
    QString weatherStatusIcon = currentWeather()->statusIconPath();

    if (temperature >= 30)
        tasksToDo.append("Bring your fan");
    if (weatherStatusIcon == "icons/rainy.png") {
        tasksToDo.append("Bring your umbrella");
        tasksToDo.append("Bring your raincoat");
    }
    if (temperature <= 20)
        tasksToDo.append("Wear your sweater");

    return tasksToDo;
}

Weather *TasksLoader::currentWeather() const
{
    return m_currentWeather;
}

void TasksLoader::setCurrentWeather(Weather *newCurrentWeather)
{
    if (m_currentWeather == newCurrentWeather)
        return;
    m_currentWeather = newCurrentWeather;
    emit currentWeatherChanged();
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

