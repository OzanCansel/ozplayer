#include "windowsservice.h"
#include <QHostInfo>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include "osinfo.h"
#include "QsLog.h"

using namespace QsLogging;

WindowsService::WindowsService(int argc, char **argv)
    :
      QtService<QCoreApplication>(argc , argv, "ozplayer")
{
    setStartupType(QtServiceController::AutoStartup);
    setServiceDescription("OzPlayer Service.");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

WindowsService::~WindowsService() { }

void WindowsService::start(){
    Logger::instance().initAsDefault("logs" , "service.log");
    Logger::instance().setLoggingLevel(QsLogging::Level::InfoLevel);
    QLOG_INFO() << "Windows service started.";
    mPlayer = QSharedPointer<PlayerService>::create();
    QSettings settings(QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini") , QSettings::IniFormat);
    auto baseDir = OsInfo().isWindows() ? QStringLiteral("C:/Users") : QStringLiteral("/home");
    QString path = settings.value(QStringLiteral("baseDir") , QVariant(baseDir)).toString();
    mPlayer->setBasePath(path);

    mPlayer->init();

    mLocator = QSharedPointer<LocatingService>::create(
                QHostAddress(QStringLiteral("239.255.43.21")) ,
                                    24944 ,
                                    mPlayer->port() ,
                                    QHostInfo::localHostName()
                );

    mLocator->init();
}

void WindowsService::stop(){

}
void WindowsService::pause(){

}
void WindowsService::resume(){

}
