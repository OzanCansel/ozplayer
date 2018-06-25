#ifndef RETRIEVELISTCOMMAND_H
#define RETRIEVELISTCOMMAND_H

#include <QStringList>
#include "iserializable.h"

class ListCommand : public ISerializable
{
public:

    static const QString CMD;
    QStringList& tracks();
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    QStringList mTracks;

};

#endif // RETRIEVELISTCOMMAND_H
