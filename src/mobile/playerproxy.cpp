#include "playerproxy.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QThread>
#include "networkutil.h"
#include "retrieveentriescommand.h"
#include "entrylistresult.h"
#include "playcommand.h"
#include "pausecommand.h"
#include "resumecommand.h"
#include "namedcommand.h"
#include "currenttracknotify.h"
#include "volumenotify.h"
#include "identifycommand.h"
#include "downloadfilecmd.h"
#include "token.h"
#include "trackpositionchanged.h"

void PlayerProxy::registerQmlType(){
    qmlRegisterType<PlayerProxy>("mobile" , 1 , 0 , "PlayerProxy");
}

PlayerProxy::PlayerProxy()
{
    connect(&mSocket , &QTcpSocket::readyRead , this , &PlayerProxy::messageIncome);
    connect(&mSocket , &QTcpSocket::disconnected , this , &PlayerProxy::playerSocketDisconnected);
    connect(&mFileSocket , &QTcpSocket::readyRead , this, &PlayerProxy::fileIncome);
    mVolume = 70;
    emit volumeChanged();
}

void PlayerProxy::open(QString host, int port){
    close();
    if(NetworkUtil::selfAddresses().contains(host))
        mSocket.connectToHost(QHostAddress::LocalHost , port);
    else
        mSocket.connectToHost(QHostAddress(host) , port);

    if(!mSocket.waitForConnected(2500)){
        return;
    }

    if(mSocket.isOpen()){
        retrieveFiles(QString());
        QTimer::singleShot(500 , this , [&]() { retrieveCurrentTrack(); });
        QTimer::singleShot(1100 , this , [&]() { retrieveId();});
        emit connectedChanged();
    }
}

void PlayerProxy::openFileService(QString host, int port){
    if(mFileSocket.isOpen()){
        mFileSocket.abort();
        mFileSocket.close();
    }
    if(NetworkUtil::selfAddresses().contains(host))
        mFileSocket.connectToHost(QHostAddress::LocalHost , port);
    else
        mFileSocket.connectToHost(QHostAddress(host) , port);

    if(!mFileSocket.waitForConnected(2500)){
        qDebug() << "Could not be connected.";
        return;
    }
}

void PlayerProxy::retrieveId(){
    mSocket.write(QJsonDocument(IdentifyCommand().serialize()).toJson());
}

void PlayerProxy::close(){
    if(mSocket.isOpen()){
        mSocket.abort();
        mSocket.close();
    }

    if(mFileSocket.isOpen()) {
        mFileSocket.abort();
        mFileSocket.close();
    }

    emit connectedChanged();
}

void PlayerProxy::playerSocketDisconnected(){
    emit disconnected();
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

        if(entriesResult.entries().length() < 2)
            return;

        auto entries = findNestedLevel(entriesResult.entries());
        qSort(entries.begin() , entries.end() ,
              [](const QPair<EntryInfo , int>& first , const QPair<EntryInfo , int>& second){
            return first.second < second.second;
        });

        auto upDir = entries.at(0);
        QVariantMap entry;
        entry["isFolder"] = upDir.first.isFolder();
        entry["path"] = upDir.first.path();
        entry["fileName"] = "Up";
        entry["isUp"] = true;

        if(upDir.second >= 0){
            mEntries.append(entry);
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
            auto fileName = QFileInfo(pair.first.path()).fileName().trimmed();

            QMap<int , int> valueScores;
            for(auto innerPair : entries){
                if(pair.first.path() == innerPair.first.path() ||
                        pair.first.isFolder())
                    continue;
                auto checkingFileName = QFileInfo(innerPair.first.path()).fileName().trimmed();
                auto score = similarityScore(fileName , checkingFileName);
                valueScores[score]++;
            }

            auto leftTrimLength = 0;
            for(auto key : valueScores.keys()){
                if(valueScores[key] > valueScores[leftTrimLength])
                    leftTrimLength = key;
            }


            if(leftTrimLength < 6)
                leftTrimLength = 0;
            entryVariant["fileName"] = fileName.right(fileName.length() - leftTrimLength);
            entryVariant["isUp"] = false;

            mEntries.append(entryVariant);
            valueScores.clear();
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
    } else if(cmd == IdentifyCommand::CMD){
        IdentifyCommand identifyCmd;
        identifyCmd.deserialize(json);
        mId =  identifyCmd.id();
    } else if (cmd == TrackPositionChanged::CMD){
        TrackPositionChanged trackPositionChangedEvent;
        trackPositionChangedEvent.deserialize(json);
        mTrackPercentage = trackPositionChangedEvent.percentage();
        mTrackPosition = trackPositionChangedEvent.position();
        emit trackPercentageChanged();
        emit trackPositionChanged();
    }

    messageIncome();
}

void PlayerProxy::fileIncome(){
    if(!mFileSocket.bytesAvailable())   return;
    mFileBuffer.append(mFileSocket.readAll());

    auto startIdx = mFileBuffer.indexOf(Token::FileDownloadStartToken);
    auto endIdx = mFileBuffer.indexOf(Token::FileDownloadEndToken);
    mFileBuffer.remove(0 , startIdx);

    if(endIdx >= 0){
        mFileBuffer.remove(endIdx , mFileBuffer.length() - endIdx);
        mFileBuffer.remove(0 , Token::FileDownloadStartToken.length());
        auto userHome = downloadsDir();
        auto downloadingFile = translateToLocal(mDownloadedFileName);
        userHome.mkpath(downloadingFile.dir().absolutePath());
        QFile downloadedFile(downloadingFile.filePath());
        qDebug() << "File path " << translateToLocal(mDownloadedFileName).filePath();
        if(!downloadedFile.open(QIODevice::WriteOnly)){
            qDebug() << "Could not open";
        }
        downloadedFile.write(mFileBuffer);
        mFileBuffer.clear();
        emit fileDownloaded(mDownloadedFileName);
    }

    fileIncome();
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

void PlayerProxy::downloadFile(QString filePath){
    DownloadFileCmd downloadCmd;
    downloadCmd.setFilePath(filePath);
    mDownloadedFileName = filePath;
    mFileSocket.write(QJsonDocument(downloadCmd.serialize()).toJson());
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


int PlayerProxy::similarityScore(QString ip, QString str){
    auto score = 0;

    for(auto i = 0; i < ip.length(); ++i){
        if(str.length() == i)
            break;
        if(ip.at(i) == str.at(i))
            score++;
        else
            break;
    }

    return score;
}


QDir PlayerProxy::rootDir(){
    return QDir(mSettings.value("baseDir").toString());
}

QDir PlayerProxy::downloadsDir(){
    return QDir(QDir(mSettings.value("baseDir").toString()).filePath(QStringLiteral("downloads/%0").arg(mId)));
}

QFileInfo PlayerProxy::translateToLocal(QString filePath){
    return QFileInfo(downloadsDir().filePath(filePath));
}

bool PlayerProxy::fileExists(QString file){
    return translateToLocal(file).exists();
}

bool PlayerProxy::isFolder(QString file){
    return translateToLocal(file).isDir();
}

QString PlayerProxy::fileName(QString file){
    return translateToLocal(file).fileName();
}

double PlayerProxy::trackPercentage(){
    return mTrackPercentage;
}

int PlayerProxy::trackPosition(){
    return mTrackPosition;
}

QString PlayerProxy::getDirectoryPath(QString path){
    return rootDir().relativeFilePath(QFileInfo(rootDir().filePath(path)).dir().absolutePath());
}
