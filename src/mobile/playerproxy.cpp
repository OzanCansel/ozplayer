#include "playerproxy.h"
#include "networkutil.h"
#include "retrieveentriescommand.h"
#include "entrylistresult.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QDir>

void PlayerProxy::registerQmlType(){
    qmlRegisterType<PlayerProxy>("mobile" , 1 , 0 , "PlayerProxy");
}

PlayerProxy::PlayerProxy()
{
    connect(&mSocket , &QTcpSocket::readyRead , this , &PlayerProxy::messageIncome);
    //open(QString("192.168.42.129") , )
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

        auto entries = findNestedLevel(entriesResult.entries());
        qSort(entries.begin() , entries.end() ,
              [](const QPair<EntryInfo , int>& first , const QPair<EntryInfo , int>& second){
            return first.second < second.second;
        });

        auto upDir = entries.at(0);
        QVariantMap entry;
        entry["isFolder"] = upDir.first.isFolder();
        entry["path"] = upDir.first.path();
        entry["fileName"] = "Yukarı";

        if(upDir.second >= 0){
            mEntries.append(entry);
            emit entriesChanged();
        }

        entries.removeFirst();
        entries.removeFirst();

        qSort(entries.begin() , entries.end() ,
              [](QPair<EntryInfo , int>& first ,  QPair<EntryInfo , int>& second){
            if(first.first.isFolder() && second.first.isFolder())
                return first.first.path() < second.first.path();
            else if(!first.first.isFolder() && !second.first.isFolder())
                return first.first.path() < second.first.path();
            else
                return first.first.isFolder();
        });

        for(auto pair : entries){
            QVariantMap entryVariant;
            entryVariant["isFolder"] = pair.first.isFolder();
            entryVariant["path"] = pair.first.path().toUtf8();
            entryVariant["fileName"] = QFileInfo(pair.first.path()).fileName().toUtf8();

            mEntries.append(entryVariant);
            emit entriesChanged();
        }
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


QList<QPair<EntryInfo , int>> PlayerProxy::findNestedLevel(QList<EntryInfo>& entries){
    QList<QPair<EntryInfo , int>> pairs;

    for(auto entry : entries){
        auto path = entry.path();
        auto level = findNestedLevel(path);
        pairs.append(QPair<EntryInfo , int>(entry, level));
    }

    return pairs;
}

int PlayerProxy::findNestedLevel(QString &path){
    QStringList paths = path.split("/");
    paths.removeAll("");
    auto level = 0;
    for(auto path : paths){
        if(path == "..")
            level--;
        else if(path == ".")
            continue;
        else
            level++;
    }

    return level;
}
