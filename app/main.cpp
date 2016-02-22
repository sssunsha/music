#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "engine.h"


#ifdef Q_OS_LINUX

#elif Q_OS_WIN

#elif Q_OS_ANDROID

#elif Q_OS_IOS

#elif Q_OS_MAC

#endif




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
