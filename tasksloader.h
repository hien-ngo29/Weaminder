#ifndef TASKSLOADER_H
#define TASKSLOADER_H

#include <QObject>
#include <qqml.h>
#include <QList>

#include <iostream>
#include <map>

class TasksLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> tasksList READ tasksList WRITE setTasksList NOTIFY tasksListChanged)
    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString weatherStatus READ weatherStatus WRITE setWeatherStatus NOTIFY weatherStatusChanged)
    QML_ELEMENT

public:
    explicit TasksLoader(QObject *parent = nullptr); 

    void loadDataFile();

    Q_INVOKABLE QList<QString> getSuitableTasksWithWeather();
    void addTaskToTasksList(QString taskName);

    QList<QString> tasksList() const;
    void setTasksList(QList<QString> newTasksList);

    int temperature() const;
    void setTemperature(int newTemperature);

    QString weatherStatus() const;
    void setWeatherStatus(const QString &newWeatherStatus);

signals:

    void tasksListChanged();

    void temperatureChanged();

    void weatherStatusChanged();

private:
    QList<QString> m_tasksList;
    int m_temperature;
    QString m_weatherStatus;
};

#endif // TASKSLOADER_H
