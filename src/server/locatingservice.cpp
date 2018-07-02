#include "locatingservice.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QDebug>
#include "findplayeripcmd.h"
#include "playeripresult.h"
#include "QsLog.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace QsLogging;

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
    connect(&mSocket , &QUdpSocket::stateChanged , this , [&](QAbstractSocket::SocketState state) {
        if(state == QAbstractSocket::BoundState){
            connect(&mSocket , &QUdpSocket::readyRead , this , &LocatingService::processPendingDatagrams);
            QLOG_INFO() << "Bounded and connected";
        }
    });

    if(!mSocket.bind(static_cast<quint16>(mPort))){
        QLOG_ERROR() << "Could not bind Udp Socket.";
        return;
    }

    connect(&mBroadcastTimer , &QTimer::timeout , this , &LocatingService::broadcast);
    mBroadcastTimer.setInterval(1000);
    mBroadcastTimer.start();
    QLOG_INFO() << "LocatingService is initiated. UdpSocket listening " << mPort;
}

void LocatingService::processPendingDatagrams(){
    QByteArray datagram;

    QLOG_INFO() << "Pending datagrams";
    while(mSocket.hasPendingDatagrams()){
        datagram.resize(static_cast<int>(mSocket.pendingDatagramSize()));
        QHostAddress cliAddr;

        quint16 cliPort;
        mSocket.readDatagram(datagram.data() , datagram.size() , &cliAddr , &cliPort);
        QLOG_INFO() << "Client host " << cliAddr.toString();
        auto data = static_cast<QString>(datagram.constData());
        QLOG_INFO() << "Datagram received => " << data;
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

void LocatingService::broadcast(){
    QUdpSocket broadcastSocket;

    QRegularExpression ipMask("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");

    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)){
            foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol){
                    auto host = ipMask.match(address.ip().toString());
                    auto broadcastAddr = address.broadcast();

                    PlayerIpResult result;
                    result.setIp(host.captured(0));
                    result.setPort(mTcpServerPort);
                    result.setPcName(mPcName);

                    auto json = QJsonDocument(result.serialize()).toJson();
                    broadcastSocket.writeDatagram(json , broadcastAddr , mPort - 1);

                    QLOG_INFO() << json << " to => " << broadcastAddr.toString() << " over " << mPort - 1<< " port";
                }
            }
        }
    }

}
