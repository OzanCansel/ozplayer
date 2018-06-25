#ifndef ENTRYINFO_H
#define ENTRYINFO_H

#include "iserializable.h"
#include <QJsonObject>

class EntryInfo : public ISerializable
{

public:

    EntryInfo();
    EntryInfo(QJsonObject);
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
    void setPath(QString);
    QString path() const;
    void setIsFolder(bool);
    bool isFolder();

private:

    QString mPath;
    bool mIsFolder;

};

#endif // ENTRYINFO_H
