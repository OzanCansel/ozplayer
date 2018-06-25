#ifndef PLAYERIPRESULT_H
#define PLAYERIPRESULT_H

#include "iserializable.h"

class PlayerIpResult : public ISerializable
{

public:

    static const QString CMD;
    QString ip();
    int port();
    void setIp(QString);
    void setPort(int);
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    int mPort;
    QString mIp;

};

#endif // PLAYERIPRESULT_H
