#include "networkutil.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>

QString NetworkUtil::selfAddress(){
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        qDebug() << address.toString();
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            return address.toString();
        }
    }

    return QString();
}
