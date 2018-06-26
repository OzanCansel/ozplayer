#include "volumenotify.h"

const QString VolumeNotify::CMD = "VolumeNotify";

int VolumeNotify::volume(){
    return mVolume;
}

void VolumeNotify::setVolume(int vol){
    mVolume = vol;
}

void VolumeNotify::deserialize(const QJsonObject &json){
    mVolume = json["volume"].toInt();
}

QJsonObject VolumeNotify::serialize(){
    QJsonObject json;
    json["cmd"] = CMD;
    json["volume"] = mVolume;

    return json;
}
