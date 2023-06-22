#ifndef TASKSLOADER_H
#define TASKSLOADER_H

#include <QObject>
#include <qqml.h>
#include <QList>

#include <iostream>
#include <map>

#include "weather.h"

class TasksLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Weather *currentWeather READ currentWeather WRITE setCurrentWeather NOTIFY currentWeatherChanged)
    Q_PROPERTY(QList<QString> tasksList READ tasksList WRITE setTasksList NOTIFY tasksListChanged)
    QML_ELEMENT

public:
    explicit TasksLoader(QObject *parent = nullptr); 

    void loadDataFile();

    Q_INVOKABLE QList<QString> getSuitableTasksWithWeather();
    void addTaskToTasksList(QString taskName);

    Weather *currentWeather() const;
    void setCurrentWeather(Weather *newCurrentWeather);

    QList<QString> tasksList() const;
    void setTasksList(QList<QString> newTasksList);

signals:
    void currentWeatherChanged();

    void tasksListChanged();

private:
    Weather* m_currentWeather;

    QList<QString> m_tasksList;
};

#endif // TASKSLOADER_H
