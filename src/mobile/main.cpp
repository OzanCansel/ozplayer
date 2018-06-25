#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "serverfinder.h"
#include "playerproxy.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    ServerFinder::registerQmlType();
    PlayerProxy::registerQmlType();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
