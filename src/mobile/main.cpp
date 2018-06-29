#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serverfinder.h"
#include "playerproxy.h"
#include "networkutil.h"
#include "osinfo.h"

int main(int argc, char *argv[])
{
    qputenv("QT_ANDROID_VOLUME_KEYS", "1");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    ServerFinder::registerQmlType();
    PlayerProxy::registerQmlType();
    OsInfo osInfo;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("osInfo" , &osInfo);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
