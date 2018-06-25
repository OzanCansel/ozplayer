#include "entryinfo.h"
#include <QJsonObject>

EntryInfo::EntryInfo() {    }

EntryInfo::EntryInfo(QJsonObject json){
    deserialize(json);
}

void EntryInfo::deserialize(const QJsonObject& json){
    mPath = json[QStringLiteral("path")].toString();
    mIsFolder = json[QStringLiteral("isFolder")].toBool();
}

QJsonObject EntryInfo::serialize(){
    QJsonObject json;

    json[QStringLiteral("path")] = mPath;
    json[QStringLiteral("isFolder")] = mIsFolder;

    return json;
}

void EntryInfo::setPath(QString path){
    mPath = path;
}

QString EntryInfo::path() const{
    return mPath;
}

void EntryInfo::setIsFolder(bool isFolder){
    mIsFolder = isFolder;
}

bool EntryInfo::isFolder(){
    return mIsFolder;
}
