#include "trackpositionchanged.h"

const QString TrackPositionChanged::CMD = "TrackPositionChanged";

void TrackPositionChanged::deserialize(const QJsonObject &json){
    mPos = json["pos"].toDouble();
    mPercentage = json["perc"].toDouble();
}

QJsonObject TrackPositionChanged::serialize(){
    QJsonObject json;
    json["cmd"] = CMD;
    json["pos"] = position();
    json["perc"] = percentage();
    return json;
}

double TrackPositionChanged::position(){
    return mPos;
}

double TrackPositionChanged::percentage(){
    return mPercentage;
}

void TrackPositionChanged::setPosition(double pos){
    mPos = pos;
}

void TrackPositionChanged::setPercentage(double percentage){
    mPercentage = percentage;
}
