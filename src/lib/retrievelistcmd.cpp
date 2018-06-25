#include "retrievelistcmd.h"
#include <QJsonObject>

const QString RetrieveListCmd::CMD = "RetrieveListCmd";


void RetrieveListCmd::deserialize(const QJsonObject &){

}

QJsonObject RetrieveListCmd::serialize(){
    QJsonObject json;

    json["cmd"] = CMD;

    return json;
}
