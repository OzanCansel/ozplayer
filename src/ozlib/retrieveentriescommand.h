#ifndef RETRIEVEENTRIESCOMMAND_H
#define RETRIEVEENTRIESCOMMAND_H

#include "iserializable.h"

class RetrieveEntriesCommand : public ISerializable
{

public:

    static const QString CMD;

    void setPath(QString path);
    QString path();
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    QString mPath;

};

#endif // RETRIEVEENTRIESCOMMAND_H
