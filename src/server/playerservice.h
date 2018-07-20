#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMediaPlayer>
#include "currenttracknotify.h"
#include "playcommand.h"
#include "cuesheet.h"

class PlayerService : public QObject
{
    Q_OBJECT

public:

    int port();
    void init();
    void setBasePath(QString);
    QString basePath();
    void stop();
    void pause();
    void start();
    void play(PlayCommand&);

private slots:

    void newConnection();
    void messageIncome();
    void clientDisconnected();
    void mediaStatusChanged(QMediaPlayer::MediaStatus);
    void trackPositionChanged(qint64);

private:

    void broadcast(QJsonObject&&);
    QMediaPlayer mPlayer;
    QTcpServer  mServer;
    QList<QTcpSocket*> mClients;

    QString mBasePath;
    QString mCurrentTrack;
    TrackStatus mTrackStatus { TrackStatus::Stopped };
    QStringList mPlaylist;
    QString mPlaylistBase;

    CueSheetTrackEntry mCurrentEntry;
    CueSheet mCuesheet;
    bool mPlayingFromCuesheet { false };
    int mTrackFinishedAt { 0 };

};

#endif // PLAYERSERVICE_H
