#include "playerproxy.h"
#include "networkutil.h"
#include "retrieveentriescommand.h"
#include "entrylistresult.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>

void PlayerProxy::registerQmlType(){
    qmlRegisterType<PlayerProxy>("mobile" , 1 , 0 , "PlayerProxy");
}

PlayerProxy::PlayerProxy()
{
     connect(&mSocket , &QTcpSocket::readyRead , this , &PlayerProxy::messageIncome);
}

void PlayerProxy::open(QString host, int port){
    if(host == NetworkUtil::selfAddress())
        mSocket.connectToHost(QHostAddress::LocalHost , port);
    else
        mSocket.connectToHost(QHostAddress(host) , port);

    if(!mSocket.waitForConnected(5000)){
        return;
    }

    if(mSocket.isOpen()){
        retrieveFiles(QString());
        emit connectedChanged();
    }

}

void PlayerProxy::messageIncome(){
    auto json = QJsonDocument::fromJson(mSocket.readAll()).object();

    if(json["cmd"].isUndefined())
        return;

    auto cmd = json["cmd"].toString();

    if(cmd == EntryListResult::CMD){
        EntryListResult entriesResult;

        entriesResult.deserialize(json);

        mEntries.clear();

        QVariantMap cdUpVariant;

        cdUpVariant["isFolder"] = true;
        cdUpVariant["path"] = entriesResult.entries().at(1).path();
        cdUpVariant["fileName"] = "Yukarı";

        qDebug() << "Yukari => " << cdUpVariant["path"].toString();

        if(cdUpVariant["path"].toString() != "../")
            mEntries.append(cdUpVariant);

        //Delete first two entries
        entriesResult.entries().removeFirst();
        entriesResult.entries().removeFirst();

        for(auto entry : entriesResult.entries()){
            QVariantMap entryVariant;
            qDebug() << entry.path();

            entryVariant["isFolder"] = entry.isFolder();
            entryVariant["path"] = entry.path();
            entryVariant["fileName"] = QFileInfo(entry.path()).fileName();

            mEntries.append(entryVariant);
        }

        emit entriesChanged();
    }
}

QVariantList& PlayerProxy::entries(){
    return mEntries;
}


QString PlayerProxy::ip(){
    return mIp;
}

int PlayerProxy::port(){
    return mPort;
}

bool PlayerProxy::connected(){
    return mSocket.isOpen();
}

void PlayerProxy::retrieveFiles(QString path){
    RetrieveEntriesCommand retrieveEntries;

    retrieveEntries.setPath(path);

    mSocket.write(QJsonDocument(retrieveEntries.serialize()).toJson());
}
