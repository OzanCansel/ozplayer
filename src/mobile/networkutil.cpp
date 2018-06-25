#include "networkutil.h"
#include <QHostAddress>
#include <QNetworkInterface>

QString NetworkUtil::selfAddress(){
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            return address.toString();
        }
    }

    return QString();
}
