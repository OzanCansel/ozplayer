#include "locatingservice.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "findplayeripcmd.h"
#include "playeripresult.h"

LocatingService::LocatingService(QHostAddress addr , int port ,
                                 QHostAddress tcpServerAddr , int tcpServerPort ,
                                 QString pcName)
    :
      mAddr(addr) ,
      mTcpServerAddr(tcpServerAddr) ,
      mPort(port) ,
      mTcpServerPort(tcpServerPort) ,
      mPcName(pcName)
{   }

void LocatingService::init(){
    mSocket.bind(QHostAddress::AnyIPv4 , static_cast<quint16>(mPort) , QUdpSocket::ShareAddress);
    mSocket.joinMulticastGroup(mAddr);
    connect(&mSocket , &QUdpSocket::readyRead , this , &LocatingService::processPendingDatagrams);
}

void LocatingService::processPendingDatagrams(){
    QByteArray datagram;

    while(mSocket.hasPendingDatagrams()){
        datagram.resize(static_cast<int>(mSocket.pendingDatagramSize()));
        QHostAddress cliAddr;
        quint16 cliPort;
        mSocket.readDatagram(datagram.data() , datagram.size() , &cliAddr , &cliPort);
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

        PlayerIpResult result;
        result.setIp(mTcpServerAddr.toString());
        result.setPort(mTcpServerPort);
        result.setPcName(mPcName);
        mSocket.writeDatagram(QJsonDocument(result.serialize()).toJson() , cliAddr , cliPort);
    }
}
