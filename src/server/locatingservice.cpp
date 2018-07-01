#include "locatingservice.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QDebug>
#include "findplayeripcmd.h"
#include "playeripresult.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

LocatingService::LocatingService(QHostAddress addr , int port ,
                                 int tcpServerPort ,
                                 QString pcName)
    :
      mAddr(addr) ,
      mPort(port) ,
      mTcpServerPort(tcpServerPort) ,
      mPcName(pcName)
{   }

void LocatingService::init(){
//    mSocket.bind(QHostAddress::AnyIPv4 , static_cast<quint16>(mPort) , QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    mSocket.bind(QHostAddress::Any , static_cast<quint16>(mPort) , QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
//    foreach (auto interface, QNetworkInterface::allInterfaces()) {
//        qDebug() << "Joining " << interface.humanReadableName() << "'s group.";
//        mSocket.joinMulticastGroup(mAddr , interface);
//        qDebug() << interface.allAddresses();
//        //If cannot multicast
//        if(!(interface.flags() & QNetworkInterface::CanMulticast)){
//            qDebug() << "Cannot multicast on " << interface.humanReadableName();
//        }
//    }
    connect(&mSocket , &QUdpSocket::readyRead , this , &LocatingService::processPendingDatagrams);
    qDebug() << "LocatingService is initiated. UdpSocket listening " << mPort;
}

void LocatingService::processPendingDatagrams(){
    QByteArray datagram;

    qDebug() << "Pending datagrams";
    while(mSocket.hasPendingDatagrams()){
        datagram.resize(static_cast<int>(mSocket.pendingDatagramSize()));
        QHostAddress cliAddr;

        quint16 cliPort;
        mSocket.readDatagram(datagram.data() , datagram.size() , &cliAddr , &cliPort);
        qDebug() << "Client host " << cliAddr.toString();
        auto data = static_cast<QString>(datagram.constData());
        auto json = QJsonDocument::fromJson(data.toUtf8()).object();

        if(json.isEmpty())
            continue;

        if(json[QStringLiteral("cmd")].isUndefined() ||
              json[QStringLiteral("cmd")].isNull())
            continue;

        auto cmd = json[QStringLiteral("cmd")].toString();

        if(cmd != FindPlayerIpCmd::CMD)
            continue;

        QRegularExpression ipMask("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");

        QStringList hosts;
        foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
            QNetworkInterface::InterfaceFlags flags = netInterface.flags();
            if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)){
                foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                    if(address.ip().protocol() == QAbstractSocket::IPv4Protocol){
                        auto matched = ipMask.match(address.ip().toString());
                        if(matched.hasMatch())
                            hosts << matched.captured(0);
                    }
                }
            }
        }

        QString clientIp = cliAddr.toString();
        auto match = ipMask.match(clientIp);

        clientIp = match.captured(0);

        QString addr = bestSimilar(clientIp , hosts);

        PlayerIpResult result;
        result.setIp(addr);
        result.setPort(mTcpServerPort);
        result.setPcName(mPcName);
        mSocket.writeDatagram(QJsonDocument(result.serialize()).toJson() , cliAddr , cliPort);
    }
}


QString LocatingService::bestSimilar(QString ip, QStringList strs){
    QString returned;
    auto lastScore = -1;
    for(auto str : strs){
        auto score = 0;
        for(auto i = 0; i < ip.length(); ++i){
            if(str.length() == i)
                break;
            if(ip.at(i) == str.at(i))
                score++;
            else
                break;
        }
        if(score > lastScore)
            returned = str;
    }

    return returned;
}
