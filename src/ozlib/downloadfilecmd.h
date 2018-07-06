#ifndef DOWNLOADFILECMD_H
#define DOWNLOADFILECMD_H

#include "iserializable.h"

class DownloadFileCmd : public ISerializable
{
public:

    static const QString CMD;
    void deserialize(const QJsonObject&) override;
    QJsonObject serialize() override;
    void setFilePath(QString);
    QString filePath();

private:

    QString mFilePath;
};

#endif // DOWNLOADFILECMD_H
