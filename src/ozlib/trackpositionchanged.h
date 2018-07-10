#ifndef TRACKPOSITIONCHANGED_H
#define TRACKPOSITIONCHANGED_H

#include "iserializable.h"

class TrackPositionChanged : public ISerializable
{
public:

    static const QString CMD;
    virtual void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
    double position();
    void setPosition(double);
    void setPercentage(double);
    double percentage();

private:

    double mPos { 0 };
    double mPercentage{ 0 };

};

#endif // TRACKPOSITIONCHANGED_H
