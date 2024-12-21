#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "audio.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    Audio audio;
    engine.rootContext()->setContextProperty("audio", &audio);

    engine.loadFromModule("freq-analysis", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
