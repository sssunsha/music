#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQuickWindow>
#include "engine.h"

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    Engine * p = new Engine();
    QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
    return p;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterSingletonType<Engine>("AudioPlayer", 1, 0, "AudioPlayer", provider);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
