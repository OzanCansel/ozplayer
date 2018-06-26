#include "playerproxy.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include "networkutil.h"
#include "retrieveentriescommand.h"
#include "entrylistresult.h"
#include "playcommand.h"
#include "pausecommand.h"
#include "resumecommand.h"
#include "namedcommand.h"
#include "currenttracknotify.h"
#include "volumenotify.h"

void PlayerProxy::registerQmlType(){
    qmlRegisterType<PlayerProxy>("mobile" , 1 , 0 , "PlayerProxy");
}

PlayerProxy::PlayerProxy()
{
    connect(&mSocket , &QTcpSocket::readyRead , this , &PlayerProxy::messageIncome);
    mVolume = 70;
    emit volumeChanged();
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
        QTimer::singleShot(1000 , this , [&]() { retrieveCurrentTrack(); });
        emit connectedChanged();
    }
}

void PlayerProxy::messageIncome(){
    mBuffer.append(mSocket.readAll());

    bool found = false;

    while(mBuffer.length() && mLastIdx < mBuffer.length()){
        mBuffer.append(mSocket.readAll());
        auto c = mBuffer.at(mLastIdx);
        if(mIgnore){
            mLastIdx++;
            mIgnore = false;
            continue;
        }
        if(c == '{')
            mBracketsCount++;

        if(mBracketsCount == 0 && mLastIdx == 0){
            mBuffer.remove(0 , 1);
            continue;
        }

        if(c == '\\')
            mIgnore = true;

        if(c == '}')
            mBracketsCount--;

        if(mBracketsCount == 0){
            found = true;
            mLastIdx++;
            break;
        }

        mLastIdx++;
    }

    if(!found) {
        return;
    }

    auto jsonStr = mBuffer.left(mLastIdx);

    auto json = QJsonDocument::fromJson(jsonStr).object();
    mBuffer.remove(0 , mLastIdx);
    mLastIdx = 0;

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
        entry["isUp"] = true;

        if(upDir.second >= 0){
            mEntries.append(entry);
            emit entriesChanged();
        }

        mCurrentDirectory = entries.at(1).first.path();
        emit currentDirectoryChanged();

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
            entryVariant["isUp"] = false;

            mEntries.append(entryVariant);
        }

        emit entriesChanged();
    } else if(cmd == CurrentTrackNotify::CMD){
        CurrentTrackNotify ctNotify;
        ctNotify.deserialize(json);

        mCurrentTrack = ctNotify.path();
        mTrackStatus = (int)ctNotify.status();

        emit currentTrackChanged();
        emit trackStatusChanged();
    } else if(cmd == VolumeNotify::CMD){
        VolumeNotify volumeNotify;
        volumeNotify.deserialize(json);
        mVolume = volumeNotify.volume();
        emit volumeChanged();
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

QString PlayerProxy::currentTrack(){
    return mCurrentTrack;
}

int PlayerProxy::trackStatus(){
    return mTrackStatus;
}

int PlayerProxy::volume(){
    return mVolume;
}

QString PlayerProxy::currentDirectory(){
    return mCurrentDirectory;
}

void PlayerProxy::retrieveFiles(QString path){
    RetrieveEntriesCommand retrieveEntries;

    retrieveEntries.setPath(path);

    mSocket.write(QJsonDocument(retrieveEntries.serialize()).toJson());
}

void PlayerProxy::retrieveCurrentTrack(){
    NamedCommand currentTrackCmd("getCurrentTrack");

    mSocket.write(QJsonDocument(currentTrackCmd.serialize()).toJson());
}

void PlayerProxy::play(QString file){
    PlayCommand playCmd;

    playCmd.setTrack(file);

    mSocket.write(QJsonDocument(playCmd.serialize()).toJson());
}

void PlayerProxy::resume(){
    ResumeCommand resumeCmd;

    mSocket.write(QJsonDocument(resumeCmd.serialize()).toJson());
}

void PlayerProxy::pause(){
    PauseCommand pauseCmd;

    mSocket.write(QJsonDocument(pauseCmd.serialize()).toJson());
}

void PlayerProxy::volumeUp(){
    if(mVolume == 100)
        return;

    NamedCommand upCmd("volumeUp");

    mSocket.write(QJsonDocument(upCmd.serialize()).toJson());
}

void PlayerProxy::volumeDown(){
    if(mVolume == 0)
        return;

    NamedCommand downCmd("volumeDown");

    mSocket.write(QJsonDocument(downCmd.serialize()).toJson());
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
