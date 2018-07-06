#include "playeripresult.h"

const QString PlayerIpResult::CMD = "PlayerIpResult";

QString PlayerIpResult::ip(){
    return mIp;
}

void PlayerIpResult::setIp(QString ip){
    mIp = ip;
}

QString PlayerIpResult::pcName(){
    return mPcName;
}

void PlayerIpResult::setPcName(QString pcName){
    mPcName = pcName;
}

int PlayerIpResult::port(){
    return mPort;
}

void PlayerIpResult::setPort(int port){
    mPort = port;
}

void PlayerIpResult::setFileServicePort(int port){
    mFileServicePort = port;
}

int PlayerIpResult::fileServicePort(){
    return mFileServicePort;
}

void PlayerIpResult::deserialize(const QJsonObject &json){
    mPort = json["p"].toInt();
    mFileServicePort = json["fP"].toInt();
    mIp = json["ip"].toString();
    mPcName = json["pn"].toString();
}

QJsonObject PlayerIpResult::serialize(){
    QJsonObject json;

    json["cmd"] = CMD;
    json["p"] = mPort;
    json["fP"] = mFileServicePort;
    json["ip"] = mIp;
    json["pn"] = mPcName;

    return json;
}
