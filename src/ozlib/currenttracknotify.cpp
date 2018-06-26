#include "currenttracknotify.h"

const QString CurrentTrackNotify::CMD = "CurrentTrackNotify";

void CurrentTrackNotify::setPath(QString path){
    mPath = path;
}

QString CurrentTrackNotify::path(){
    return mPath;
}

void CurrentTrackNotify::setStatus(TrackStatus status){
    mStatus = status;
}

TrackStatus CurrentTrackNotify::status(){
    return mStatus;
}

void CurrentTrackNotify::deserialize(const QJsonObject &json){
    mPath = json["path"].toString();
    mStatus = (TrackStatus)json["status"].toInt();
}

QJsonObject CurrentTrackNotify::serialize(){
    QJsonObject json;

    json["cmd"] = CMD;
    json["path"] = mPath;
    json["status"] = (int)mStatus;

    return json;
}
