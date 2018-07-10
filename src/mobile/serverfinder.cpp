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
    connect(&mListener , &QUdpSocket::readyRead , this , &ServerFinder::ipReceived);
    connect(this , &ServerFinder::enabledChanged , this , &ServerFinder::checkListenerSocket);

    if(mListener.bind(24943)){
        qDebug() << "Listener socket could not bind";
    }
}

QVariantList ServerFinder::servers(){
    return mServers;
}

void ServerFinder::search(){
    if(!mSocket.isOpen())
        mSocket.open(QIODevice::ReadWrite);

    auto datagram = QJsonDocument(FindPlayerIpCmd().serialize()).toJson();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
                mSocket.writeDatagram(datagram , broadcastAddress , 24944);
            }
        }
    }
}

void ServerFinder::searchTimedout(){
    if(mSocket.isOpen())
        mSocket.close();

    emit searchTimedout();
}

void ServerFinder::checkListenerSocket(){
    if(isEnabled()){
        mListener.bind(24943);
    } else {
        mListener.abort();
    }
}

void ServerFinder::ipReceived(){
    qDebug() << "Received";
    QByteArray datagram;
    datagram.resize(int(mListener.pendingDatagramSize()));
    mListener.readDatagram(datagram.data() , datagram.size());
    auto json = QJsonDocument::fromJson(datagram).object();

    if(json.isEmpty())
        return;

    if(!json.contains(QStringLiteral("cmd")))
        return;

    auto cmd = json[QStringLiteral("cmd")].toString();

    if(cmd == PlayerIpResult::CMD){
        PlayerIpResult ipResult;
        ipResult.deserialize(json);

        if(exists(ipResult))
            return;
        if(ipResult.pcName().isEmpty())
            return;
        QVariantMap variant;
        variant["ip"] = ipResult.ip();
        variant["port"] = ipResult.port();
        variant["pcName"] = ipResult.pcName();
        variant["fileServerPort"] = ipResult.fileServicePort();
        mServers.append(variant);
        emit serversChanged();
    }
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
        if(exists(ipResult))
            return;
        if(ipResult.pcName().isEmpty())
            return;
        QVariantMap variant;
        variant["ip"] = ipResult.ip();
        variant["port"] = ipResult.port();
        variant["pcName"] = ipResult.pcName();
        mServers.append(variant);
        emit serversChanged();
    }
}

bool ServerFinder::exists(PlayerIpResult &ip){
    for(auto variant : mServers){
        if(variant.toMap()["ip"].toString() == ip.ip() &&
                variant.toMap()["port"].toInt() == ip.port())
            return true;
    }

    return false;
}
