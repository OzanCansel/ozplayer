#include "resumecommand.h"

const QString ResumeCommand::CMD = "ResumeCommand";

void ResumeCommand::deserialize(const QJsonObject &){

}

QJsonObject ResumeCommand::serialize(){
    QJsonObject json;
    json["cmd"] = CMD;
    return json;
}
