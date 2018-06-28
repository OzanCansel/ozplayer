#include "windowsservice.h"
#include <QHostInfo>
#include <QCoreApplication>
#include <QStandardPaths>

WindowsService::WindowsService(int argc, char **argv)
    :
      QtService<QCoreApplication>(argc , argv, "ozplayer")
{
    setStartupType(QtServiceController::AutoStartup);
    setServiceDescription("A Qt service with user interface.");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

WindowsService::~WindowsService() { }

void WindowsService::start(){
    mPlayer = QSharedPointer<PlayerService>::create();
    mPlayer->setBasePath(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));

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
