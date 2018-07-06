#include "identifycommand.h"

const QString IdentifyCommand::CMD = "identifyCmd";

void IdentifyCommand::deserialize(const QJsonObject &json){
    mId = json["id"].toString();
}

QJsonObject IdentifyCommand::serialize(){
    QJsonObject json;

    json["cmd"] = CMD;
    json["id"] = mId;

    return json;
}

void IdentifyCommand::setId(QString id){
    mId = id;
}

QString IdentifyCommand::id(){
    return mId;
}
