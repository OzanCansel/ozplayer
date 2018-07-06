#ifndef IDENTIFYCOMMAND_H
#define IDENTIFYCOMMAND_H

#include "iserializable.h"

class IdentifyCommand : public ISerializable
{

public:

    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
    void setId(QString id);
    QString id();

private:

    QString mId;

};

#endif // IDENTIFYCOMMAND_H
