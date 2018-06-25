#include "serverfinder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "playeripresult.h"
#include "findplayeripcmd.h"
#include <QHostAddress>
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
    mSocket.writeDatagram(datagram , QHostAddress(QStringLiteral("239.255.43.21")) , 45454);
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

