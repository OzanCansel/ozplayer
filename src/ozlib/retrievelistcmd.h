#ifndef RETRIEVELISTCMD_H
#define RETRIEVELISTCMD_H

#include "iserializable.h"

class RetrieveListCmd : public ISerializable
{

public:

    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

};

#endif // RETRIEVELISTCMD_H
