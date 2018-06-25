#include "entrylistresult.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "entryinfo.h"

const QString EntryListResult::CMD = "EntryListResult";

void EntryListResult::deserialize(const QJsonObject& json){
    auto entries = json[QStringLiteral("entries")].toArray();

    for(const QJsonValue& val : entries){
        auto entryObj = val.toObject();
        mEntries.append(EntryInfo(entryObj));
    }

}

QJsonObject EntryListResult::serialize(){
    QJsonObject json;
    QJsonArray entries;
    json[QStringLiteral("cmd")] = CMD;
    for(auto entry : mEntries){
        entries.append(entry.serialize());
    }

    json[QStringLiteral("entries")] = entries;

    return json;
}

QList<EntryInfo>& EntryListResult::entries(){
    return mEntries;
}

void EntryListResult::setEntries(QList<EntryInfo> entries){
    mEntries = entries;
}
