#include "retrieveentriescommand.h"
#include <QJsonObject>

const QString RetrieveEntriesCommand::CMD = "RetrieveEntriesCmd";

void RetrieveEntriesCommand::setPath(QString path){
    mPath = path;
}

QString RetrieveEntriesCommand::path(){
    return mPath;
}

void RetrieveEntriesCommand::deserialize(const QJsonObject& json){
    mPath = json["path"].toString();
}

QJsonObject RetrieveEntriesCommand::serialize(){
    QJsonObject json;

    json[QStringLiteral("cmd")] = CMD;
    json[QStringLiteral("path")] = mPath;

    return json;
}
