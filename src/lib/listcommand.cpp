#include "listcommand.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

const QString ListCommand::CMD = "ListCommand";

QStringList& ListCommand::tracks(){
    return mTracks;
}

void ListCommand::deserialize(const QJsonObject &json){
    mTracks.clear();

    auto tracks = json[QStringLiteral("trakcs")].toArray();

    for(auto track : tracks){
        mTracks.append(track.toString());
    }

}

QJsonObject ListCommand::serialize(){
    QJsonArray tracks;

    for(auto track : mTracks)
        tracks.append(track);

    QJsonObject json;

    json["cmd"] = CMD;
    json["tracks"] = tracks;

    return json;
}
