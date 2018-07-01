#include "windowsservice.h"
#include <QHostInfo>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include "osinfo.h"

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
    mPlayer = QSharedPointer<PlayerService>::create();
    QSettings settings(QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini") , QSettings::IniFormat);
    auto baseDir = OsInfo().isWindows() ? QStringLiteral("C:/Users") : QStringLiteral("/home");
    QString path = settings.value(QStringLiteral("baseDir") , QVariant(baseDir)).toString();
    mPlayer->setBasePath(path);

    mPlayer->init();

    mLocator = QSharedPointer<LocatingService>::create(
                QHostAddress(QStringLiteral("239.255.43.21")) ,
                                    45454 ,
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
