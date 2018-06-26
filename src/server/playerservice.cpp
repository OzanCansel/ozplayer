#include "playerservice.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "retrieveentriescommand.h"
#include "entrylistresult.h"
#include "entryinfo.h"
#include "playcommand.h"
#include "pausecommand.h"
#include "resumecommand.h"
#include "volumenotify.h"

void PlayerService::init(){
    connect(&mServer , &QTcpServer::newConnection , this , &PlayerService::newConnection);
    mServer.listen();
}

int PlayerService::port(){
    return static_cast<int>(mServer.serverPort());
}

void PlayerService::newConnection(){
    if(!mServer.hasPendingConnections())
        return;

    auto socket = mServer.nextPendingConnection();
    connect(socket , &QTcpSocket::readyRead , this , &PlayerService::messageIncome);
    connect(socket , &QTcpSocket::disconnected , this , &PlayerService::clientDisconnected);

    mClients.append(socket);
}

void PlayerService::clientDisconnected(){
    auto socket = static_cast<QTcpSocket*>(QObject::sender());
    //Remove from list
    mClients.removeOne(socket);
    //Delete socket
    socket->deleteLater();
}

void PlayerService::setBasePath(QString path){
    mBasePath = path;
}

QString PlayerService::basePath(){
    return mBasePath;
}

void PlayerService::messageIncome(){
    auto socket = static_cast<QTcpSocket*>(QObject::sender());
    auto json = QJsonDocument::fromJson(socket->readAll()).object();

    //Validate json
    if(!json.contains(QStringLiteral("cmd")))
        return;

    auto cmd = json[QStringLiteral("cmd")].toString();

    if(cmd == RetrieveEntriesCommand::CMD){
        RetrieveEntriesCommand retrieveEntriesCmd;

        //Deserialize
        retrieveEntriesCmd.deserialize(json);

        qDebug() << retrieveEntriesCmd.path().toUtf8();

        QDir requestedDir(mBasePath);
        requestedDir.cd(retrieveEntriesCmd.path());

        requestedDir.setSorting(QDir::DirsFirst);
        auto entries = requestedDir.entryInfoList();

        EntryListResult result;
        for(QFileInfo entry : entries){
            if(!entry.isDir() && !entry.suffix().contains("MP3" , Qt::CaseInsensitive) &&
                            !entry.suffix().contains("flac" , Qt::CaseInsensitive))
                continue;

            EntryInfo entryInfo;
            entryInfo.setIsFolder(entry.isDir());
            entryInfo.setPath(QDir(mBasePath).relativeFilePath(entry.filePath().toUtf8()));
            result.entries().append(entryInfo);
            qDebug() << "   " << entryInfo.path() << " added.";
        }

        socket->write(QJsonDocument(result.serialize()).toJson());
    } else if(cmd == PlayCommand::CMD){
        PlayCommand playCmd;
        playCmd.deserialize(json);
        QDir baseDir(mBasePath);
        mCurrentTrack = baseDir.filePath(playCmd.track());

        mPlayer.setMedia(QUrl::fromLocalFile(mCurrentTrack));
        mPlayer.play();

        mTrackStatus = TrackStatus::Playing;

        CurrentTrackNotify notify;
        notify.setStatus(mTrackStatus);
        notify.setPath(playCmd.track());
        broadcast(notify.serialize());
    } else if(cmd == ResumeCommand::CMD){
        if(mPlayer.state() != QMediaPlayer::PlayingState){
            mPlayer.play();

            mTrackStatus = TrackStatus::Playing;

            CurrentTrackNotify notify;

            notify.setStatus(mTrackStatus);
            notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
            broadcast(notify.serialize());
        }
    } else if(cmd == PauseCommand::CMD){
        if(mPlayer.state() != QMediaPlayer::PausedState){
            mPlayer.pause();
            mTrackStatus = TrackStatus::Paused;
            CurrentTrackNotify notify;
            notify.setStatus(mTrackStatus);
            notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
            broadcast(notify.serialize());
        }
    } else if(cmd == "volumeUp"){
        auto volume = mPlayer.volume() + 5;
        if(volume > 100)
            volume = 100;
        mPlayer.setVolume(volume);
        VolumeNotify notify;
        notify.setVolume(mPlayer.volume());
        broadcast(notify.serialize());
        qDebug() << "volumeUp => " << volume;
    } else if(cmd == "volumeDown"){
        auto volume = mPlayer.volume() - 5;
        if(volume < 0)
            volume = 0;
        mPlayer.setVolume(volume);
        VolumeNotify notify;
        notify.setVolume(mPlayer.volume());
        broadcast(notify.serialize());
        qDebug() << "volumeDown => " << volume;
    } else if(cmd == "getCurrentTrack"){
        CurrentTrackNotify notify;
        notify.setStatus(mTrackStatus);
        notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
        broadcast(notify.serialize());
    }
}

void PlayerService::broadcast(QJsonObject&& json){
    auto str = QJsonDocument(json).toJson();
    for(auto socket : mClients)
        socket->write(str);}
