#include "namedcommand.h"

NamedCommand::NamedCommand(QString name)
    :
      mName(name)
{   }


void NamedCommand::deserialize(const QJsonObject &json){

}

QJsonObject NamedCommand::serialize(){
    QJsonObject json;
    json["cmd"] = mName;
    return json;
}
