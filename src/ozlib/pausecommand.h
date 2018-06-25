#ifndef PAUSECOMMAND_H
#define PAUSECOMMAND_H

#include "iserializable.h"

class PauseCommand : public ISerializable
{
public:
    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
};

#endif // PAUSECOMMAND_H
