#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMediaPlayer>
#include "currenttracknotify.h"
#include "playcommand.h"

class PlayerService : public QObject
{
    Q_OBJECT

public:

    int port();
    void init();
    void newConnection();
    void setBasePath(QString);
    QString basePath();

private slots:

    void messageIncome();
    void clientDisconnected();
    void mediaStatusChanged(QMediaPlayer::MediaStatus);
    void play(PlayCommand&);

private:

    void broadcast(QJsonObject&&);

    QTcpServer  mServer;
    QList<QTcpSocket*> mClients;
    QString mBasePath;
    QMediaPlayer mPlayer;
    QString mCurrentTrack;
    TrackStatus mTrackStatus;
    QStringList mPlaylist;
    QString mPlaylistBase;

};

#endif // PLAYERSERVICE_H
