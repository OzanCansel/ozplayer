#ifndef NAMEDCOMMAND_H
#define NAMEDCOMMAND_H

#include "iserializable.h"

class NamedCommand : public ISerializable
{
public:

    NamedCommand(QString);
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    QString mName;

};

#endif // NAMEDCOMMAND_H
