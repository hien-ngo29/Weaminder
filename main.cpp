#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QGuiApplication>
#include <QIcon>

#include "weather.h"
#include "datetime.h"
#include "tasksloader.h"
#include "weaminder.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<Weaminder>("com.mycompany.weaminder", 1, 0, "Weaminder");
    qmlRegisterType<Weather>("com.mycompany.weather", 1, 0, "Weather");
    qmlRegisterType<DateTime>("com.mycompany.datetime", 1, 0, "DateTime");
    qmlRegisterType<TasksLoader>("com.mycompany.task", 1, 0, "TasksLoader");

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon("../icon.png"));

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
