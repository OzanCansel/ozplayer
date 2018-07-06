#include "downloadfilecmd.h"

const QString DownloadFileCmd::CMD = "downloadFile";

void DownloadFileCmd::deserialize(const QJsonObject &json){
    mFilePath = json["filePath"].toString();
}

QJsonObject DownloadFileCmd::serialize(){
    QJsonObject json;

    json["cmd"] = CMD;
    json["filePath"] = mFilePath;

    return json;
}

void DownloadFileCmd::setFilePath(QString path){
    mFilePath = path;
}

QString DownloadFileCmd::filePath(){
    return mFilePath;
}
