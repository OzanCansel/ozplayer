#include "networkutil.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>

QStringList NetworkUtil::selfAddresses(){
    QStringList list;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            list << address.toString();
        }
    }

    return list;
}
