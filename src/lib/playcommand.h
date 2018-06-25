#ifndef PLAYCOMMAND_H
#define PLAYCOMMAND_H

#include "iserializable.h"

class PlayCommand : public ISerializable
{

public:

    static const QString CMD;
    void setTrack(QString);
    QString track();
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    QString mTrack;

};

#endif // PLAYCOMMAND_H
