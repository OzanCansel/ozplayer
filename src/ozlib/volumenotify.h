#ifndef VOLUMENOTIFY_H
#define VOLUMENOTIFY_H

#include "iserializable.h"

class VolumeNotify : public ISerializable
{
public:

    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
    void setVolume(int);
    int volume();

private:

    int mVolume {0};
};

#endif // VOLUMENOTIFY_H
