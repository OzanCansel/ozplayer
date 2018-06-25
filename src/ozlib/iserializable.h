#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <QJsonObject>

class ISerializable
{
public:

    //virtual ~ISerializable();
    virtual void deserialize(const QJsonObject&) = 0;
    virtual QJsonObject serialize() = 0;

};

#endif // ISERIALIZABLE_H
