#include "findplayeripcmd.h"
#include <QJsonObject>

const QString FindPlayerIpCmd::CMD = "FindPlayerIpCmd";

void FindPlayerIpCmd::deserialize(const QJsonObject &json){

}

QJsonObject FindPlayerIpCmd::serialize(){
    QJsonObject json;
    json["cmd"] = CMD;
    return json;
}
