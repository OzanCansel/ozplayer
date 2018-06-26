#ifndef CURRENTTRACKNOTIFY_H
#define CURRENTTRACKNOTIFY_H

#include "iserializable.h"

enum class TrackStatus { Playing = 0 , Paused = 1 , Stopped = 2 };

class CurrentTrackNotify : public ISerializable
{

public:

    static const QString CMD;
    void setPath(QString);
    QString path();
    void setStatus(TrackStatus);
    TrackStatus status();
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    QString mPath;
    TrackStatus mStatus { TrackStatus::Stopped };

};

#endif // CURRENTTRACKNOTIFY_H
