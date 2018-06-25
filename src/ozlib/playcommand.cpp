#include "playcommand.h"

const QString PlayCommand::CMD = "PlayCommand";

void PlayCommand::setTrack(QString track){
    mTrack = track;
}

QString PlayCommand::track(){
    return mTrack;
}

void PlayCommand::deserialize(const QJsonObject &json){
    mTrack = json["track"].toString();
}

QJsonObject PlayCommand::serialize(){
    QJsonObject json;
    json["track"] = mTrack;
    json["cmd"] = CMD;
    return json;
}
