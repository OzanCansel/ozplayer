#include "playerservice.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include "retrieveentriescommand.h"
#include "entrylistresult.h"
#include "entryinfo.h"
#include "playcommand.h"
#include "pausecommand.h"
#include "resumecommand.h"
#include "volumenotify.h"
#include "identifycommand.h"
#include "trackpositionchanged.h"
#include "QsLog.h"

using namespace QsLogging;

void PlayerService::init(){
    connect(&mServer , &QTcpServer::newConnection , this , &PlayerService::newConnection);
    connect(&mPlayer , &QMediaPlayer::mediaStatusChanged , this , &PlayerService::mediaStatusChanged);
    connect(&mPlayer , &QMediaPlayer::positionChanged , this , &PlayerService::trackPositionChanged);
    mPlayer.setNotifyInterval(500);
    mServer.listen();

    QLOG_INFO() << "PlayerService initiated. TcpServer listening " << port();
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

void PlayerService::start(){
    if(mPlayer.state() != QMediaPlayer::PlayingState){
        mPlayer.play();

        mTrackStatus = TrackStatus::Playing;

        CurrentTrackNotify notify;

        notify.setStatus(mTrackStatus);
        notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
        broadcast(notify.serialize());
    }
}

void PlayerService::stop(){
    if(mPlayer.state() != QMediaPlayer::PausedState){
        mPlayer.pause();
        mTrackStatus = TrackStatus::Paused;
        CurrentTrackNotify notify;
        notify.setStatus(mTrackStatus);
        notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
        broadcast(notify.serialize());
    }
}

void PlayerService::pause(){
    if(mPlayer.state() != QMediaPlayer::PausedState){
        mPlayer.pause();
        mTrackStatus = TrackStatus::Paused;
        CurrentTrackNotify notify;
        notify.setStatus(mTrackStatus);
        notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
        broadcast(notify.serialize());
    }
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

void PlayerService::mediaStatusChanged(QMediaPlayer::MediaStatus status){
    if(status == QMediaPlayer::EndOfMedia){
        auto idx = mPlaylist.indexOf(mCurrentTrack);
        idx = mPlaylist.length() - 1 <= idx ? 0 : idx + 1;
        PlayCommand playCmd;
        playCmd.setTrack(QDir(mBasePath).relativeFilePath(mPlaylist.at(idx)));
        play(playCmd);
    }
}

void PlayerService::play(PlayCommand& playCmd){
    QDir baseDir(mBasePath);
    QFileInfo trackFInfo(baseDir.filePath(playCmd.track()));
    int mediaPosition = 0 ;
    //If track doesn't exist, check cuesheet whether they contain the track
//    if(!trackFInfo.exists()){
//        for(QFileInfo entry : trackFInfo.dir().entryInfoList(QStringList() << "*.cue")){
//            if(!entry.suffix().contains("cue" , Qt::CaseInsensitive))
//                continue;
//            auto cuesheet = CueSheet::loadFrom(entry.filePath());

//            //Iterate cuesheet entries
//            for(auto trackEntry : cuesheet.entries()){

//                if(playCmd.track().contains(trackEntry.displayName())){
//                    mCurrentTrack = cuesheet.musicFile();
//                    mediaPosition = trackEntry.startIndexes().first().toMillis();
//                    mPlaylist.clear();
//                    mPlaylist.append(mCurrentTrack);
//                    mPlayingFromCuesheet = true;
//                    mCuesheet = cuesheet;
//                }
//            }
//        }
//    } else {
        QDir trackDir(baseDir.filePath(playCmd.track()));
        trackDir.cdUp();
        if(trackDir.path() != mPlaylistBase){
            mPlaylist.clear();
            for(QFileInfo entry : trackDir.entryInfoList(QStringList() << "*" , QDir::Files)){
                if(!entry.isDir() && !entry.suffix().contains("MP3" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("flac" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("m4a" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("wma" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("ogg" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("wav" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("ape" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("aac" , Qt::CaseInsensitive) &&
                        !entry.suffix().contains("oga" , Qt::CaseInsensitive))
                    continue;
                mPlaylist.append(entry.filePath());
            }
            mPlaylistBase = trackDir.path();
        }
        mCurrentTrack = baseDir.filePath(playCmd.track());
        mPlayingFromCuesheet = false;
//    }


    mPlayer.setMedia(QUrl::fromLocalFile(mCurrentTrack));
    mPlayer.play();
    mPlayer.setPosition(mediaPosition);

    mTrackStatus = TrackStatus::Playing;

    CurrentTrackNotify notify;
    notify.setStatus(mTrackStatus);
    notify.setPath(playCmd.track());
    broadcast(notify.serialize());
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

        QLOG_INFO() << retrieveEntriesCmd.path().toUtf8();

        QDir requestedDir(mBasePath);
        requestedDir.cd(retrieveEntriesCmd.path());

        requestedDir.setSorting(QDir::DirsFirst);
        auto entries = requestedDir.entryInfoList();

        EntryListResult result;
        QStringList ignoredFiles;

        for(QFileInfo entry : entries){
            if(!entry.suffix().contains("cue" , Qt::CaseInsensitive))
                continue;

//            auto sheet = CueSheet::loadFrom(entry.filePath());
//            if(!QFileInfo(sheet.musicFile()).exists())
//                continue;
//            if(ignoredFiles.contains(sheet.musicFile()))
//                continue;
//            ignoredFiles.append(sheet.musicFile());

//            for(auto sheetEntry : sheet.entries()){
//                EntryInfo entryInfo;
//                entryInfo.setIsFolder(false);
//                entryInfo.setPath(QDir(mBasePath).relativeFilePath(entry.dir().filePath(sheetEntry.displayName())));
//                if(ignoredFiles.contains(entryInfo.path()))
//                    continue;
//                result.entries().append(entryInfo);
//                ignoredFiles << entryInfo.path();
//            }
        }


        for(QFileInfo entry : entries){
            if(!entry.isDir() && !entry.suffix().contains("MP3" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("flac" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("m4a" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("wma" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("ogg" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("wav" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("ape" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("aac" , Qt::CaseInsensitive) &&
                    !entry.suffix().contains("oga" , Qt::CaseInsensitive))
                continue;

            //Check ignored files
            if(ignoredFiles.contains(entry.filePath()))
                continue;

            EntryInfo entryInfo;
            entryInfo.setIsFolder(entry.isDir());
            entryInfo.setPath(QDir(mBasePath).relativeFilePath(entry.filePath().toUtf8()));
            result.entries().append(entryInfo);
            QLOG_INFO() << "   " << entryInfo.path() << " added.";
        }

        socket->write(QJsonDocument(result.serialize()).toJson());
    } else if(cmd == PlayCommand::CMD){
        PlayCommand playCmd;
        playCmd.deserialize(json);
        play(playCmd);
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
        QLOG_INFO() << "volumeUp => " << volume;
    } else if(cmd == "volumeDown"){
        auto volume = mPlayer.volume() - 5;
        if(volume < 0)
            volume = 0;
        mPlayer.setVolume(volume);
        VolumeNotify notify;
        notify.setVolume(mPlayer.volume());
        broadcast(notify.serialize());
        QLOG_INFO() << "volumeDown => " << volume;
    } else if(cmd == "getCurrentTrack"){
        CurrentTrackNotify notify;
        notify.setStatus(mTrackStatus);
        notify.setPath(QDir(mBasePath).relativeFilePath(mCurrentTrack));
        broadcast(notify.serialize());
    } else if(cmd == IdentifyCommand::CMD){
        IdentifyCommand cmd;
        auto id = QSysInfo::machineHostName();
        cmd.setId(id);
        socket->write(QJsonDocument(cmd.serialize()).toJson());
        socket->waitForBytesWritten();
    }
}

void PlayerService::broadcast(QJsonObject&& json){
    auto str = QJsonDocument(json).toJson();
    for(auto socket : mClients)
        socket->write(str);

    for(auto socket : mClients)
        socket->waitForBytesWritten();
}

void PlayerService::trackPositionChanged(qint64 pos){
    TrackPositionChanged event;

//    if(mPlayingFromCuesheet){
//        if(pos == 0)
//            return;
//        auto entry = mCuesheet.getEntryByPos(pos);
//        if(entry.title().isEmpty())
//            return;
//        qDebug() << entry.title();
//        if(entry.title() != mCurrentEntry.title()){
//            mCurrentEntry = entry;

//            CurrentTrackNotify notify;
//            notify.setStatus(mTrackStatus);
//            notify.setPath(QDir(mBasePath).relativeFilePath(QFileInfo(mCuesheet.musicFile()).dir().filePath(mCurrentEntry.displayName())));
//            broadcast(notify.serialize());
//        }

//        auto start = mCurrentEntry.startIndexes()[0].toMillis();
//        auto finish = mCurrentEntry.finishIndex().toMillis();
//        auto posInSecs = (pos - start) / 1000;
//        auto percentage = mPlayer.duration() > 0.0 ? static_cast<double>(pos) / mPlayer.duration() : 0;
//        if(finish < start){
//            percentage = mPlayer.duration() > 0 ? (pos - start) / static_cast<double>(mPlayer.duration() - start) : 0;
//        } else {
//            percentage = (pos - start) / static_cast<double>(finish - start);
//        }

//        event.setPosition(posInSecs);
//        event.setPercentage(percentage);
//    } else {
        auto posInSecs = pos / 1000;
        auto percentage = mPlayer.duration() > 0.0 ? static_cast<double>(pos) / mPlayer.duration() : 0;
        event.setPosition(posInSecs);
        event.setPercentage(percentage);
//    }

    broadcast(event.serialize());
}
