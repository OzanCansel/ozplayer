#ifndef PLAYERIPRESULT_H
#define PLAYERIPRESULT_H

#include "iserializable.h"

class PlayerIpResult : public ISerializable
{

public:

    static const QString CMD;
    QString ip();
    int port();
    int fileServicePort();
    void setIp(QString);
    void setPort(int);
    void setPcName(QString);
    void setFileServicePort(int);
    QString pcName();
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;

private:

    int mPort;
    int mFileServicePort;
    QString mIp;
    QString mPcName;

};

#endif // PLAYERIPRESULT_H
