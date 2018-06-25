#ifndef FINDPLAYERIPCMD_H
#define FINDPLAYERIPCMD_H

#include "iserializable.h"

class FindPlayerIpCmd : public ISerializable
{

public:

    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

};

#endif // FINDPLAYERIPCMD_H
