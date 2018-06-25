#ifndef ENTRYLISTRESULT_H
#define ENTRYLISTRESULT_H

#include "iserializable.h"
#include "entryinfo.h"
#include <QFileInfoList>

class EntryListResult : public ISerializable
{

public:

    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
    QList<EntryInfo>& entries();
    void setEntries(QList<EntryInfo>);

private:

    QList<EntryInfo> mEntries;

};

#endif // ENTRYLISTRESULT_H
