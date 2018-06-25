#include <QCoreApplication>
#include <QStandardPaths>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include "playerservice.h"
#include "locatingservice.h"

QHostAddress ip(){
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            return address;
        }
    }

    return QHostAddress();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PlayerService service;
    service.setBasePath(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));
    service.init();
    qDebug() << "Player service is listening on " << service.port();

    LocatingService locatingService(QHostAddress(QStringLiteral("239.255.43.21")) ,
                                    45454 ,
                                    ip() ,
                                    service.port() ,
                                    QHostInfo::localHostName()
                                    );
    locatingService.init();

    return a.exec();
}
