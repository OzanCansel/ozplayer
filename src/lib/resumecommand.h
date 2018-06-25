#ifndef RESUMECOMMAND_H
#define RESUMECOMMAND_H

#include "iserializable.h"

class ResumeCommand : public ISerializable
{
public:
    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
};

#endif // RESUMECOMMAND_H
