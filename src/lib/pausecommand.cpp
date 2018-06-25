#include "pausecommand.h"

const QString PauseCommand::CMD = "PauseCmd";

void PauseCommand::deserialize(const QJsonObject &){

}

QJsonObject PauseCommand::serialize(){
    QJsonObject json;
    json["cmd"] = CMD;
    return json;
}
