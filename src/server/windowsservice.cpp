#include "windowsservice.h"
#include <QHostInfo>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include "osinfo.h"
#include "QsLog.h"
#include <QProcess>

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

    QLOG_INFO() << "ozplayer service is started.";

    //Player service init
    mPlayerService = QSharedPointer<PlayerService>::create();
    mPlayerService->stop();
    QSettings settings(QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini") , QSettings::IniFormat);
    auto baseDir = OsInfo().isWindows() ? QStringLiteral("C:/Users") : QStringLiteral("/home");
    QString path = settings.value(QStringLiteral("baseDir") , QVariant(baseDir)).toString();
    mPlayerService->setBasePath(path);
    mPlayerService->init();

    //File service init
    mFileService = QSharedPointer<FileService>::create();
    mFileService->setBasePath(path);
    mFileService->init();


    //Locator service init
    mLocatorService = QSharedPointer<LocatingService>::create(
                QHostAddress(QStringLiteral("239.255.43.21")) ,
                                    24944 ,
                                    mPlayerService->port() ,
                                    mFileService->port() ,
                                    QHostInfo::localHostName()
                );

    mLocatorService->init();
}

void WindowsService::stop(){
    mPlayerService->stop();
}
void WindowsService::pause(){
    mPlayerService->pause();
}
void WindowsService::resume(){
    mPlayerService->start();
}
