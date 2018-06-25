#include "playeripresult.h"

const QString PlayerIpResult::CMD = "PlayerIpResult";

QString PlayerIpResult::ip(){
    return mIp;
}

void PlayerIpResult::setIp(QString ip){
    mIp = ip;
}

int PlayerIpResult::port(){
    return mPort;
}

void PlayerIpResult::setPort(int port){
    mPort = port;
}

void PlayerIpResult::deserialize(const QJsonObject &json){
    mPort = json["port"].toInt();
    mIp = json["ip"].toString();
}

QJsonObject PlayerIpResult::serialize(){
    QJsonObject json;

    json["cmd"] = CMD;
    json["port"] = mPort;
    json["ip"] = mIp;

    return json;
}
