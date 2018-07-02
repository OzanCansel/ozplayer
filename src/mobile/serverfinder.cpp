#include "serverfinder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "playeripresult.h"
#include "findplayeripcmd.h"
#include <QHostAddress>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include "networkutil.h"

void ServerFinder::registerQmlType(){
    qmlRegisterType<ServerFinder>("mobile" , 1 , 0 , "ServerFinder");
}

ServerFinder::ServerFinder(){
    connect(&mSocket , &QUdpSocket::readyRead , this , &ServerFinder::messageIncome);
}

QVariantList ServerFinder::servers(){
    return mServers;
}

void ServerFinder::search(){
    if(!mSocket.isOpen())
        mSocket.open(QIODevice::ReadWrite);

    auto datagram = QJsonDocument(FindPlayerIpCmd().serialize()).toJson();
//    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
//        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
//            QHostAddress broadcastAddress = entry.broadcast();
//            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
//                mSocket.writeDatagram(datagram , broadcastAddress , 24944);
//            }
//        }
//    }

//    mSocket.writeDatagram(datagram , QHostAddress::Broadcast , 24944);
    mSocket.bind(24941);
}

void ServerFinder::searchTimedout(){
    if(mSocket.isOpen())
        mSocket.close();

    emit searchTimedout();
}

void ServerFinder::messageIncome(){
    QByteArray datagram;
    datagram.resize(int(mSocket.pendingDatagramSize()));
    mSocket.readDatagram(datagram.data() , datagram.size());
    auto json = QJsonDocument::fromJson(datagram).object();

    if(json.isEmpty())
        return;

    if(!json.contains(QStringLiteral("cmd")))
        return;

    auto cmd = json[QStringLiteral("cmd")].toString();

    if(cmd == PlayerIpResult::CMD){
        PlayerIpResult ipResult;

        ipResult.deserialize(json);
        QVariantMap variant;
        variant["ip"] = ipResult.ip();
        variant["port"] = ipResult.port();
        variant["pcName"] = ipResult.pcName();
        mServers.append(variant);
        emit serversChanged();
    }
}

