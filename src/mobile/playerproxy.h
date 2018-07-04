#ifndef PLAYERPROXY_H
#define PLAYERPROXY_H

#include <QQuickItem>
#include <QTcpSocket>
#include <QMap>
#include <QPair>
#include "entrylistresult.h"

class PlayerProxy : public QQuickItem
{

    Q_OBJECT
    Q_PROPERTY(QString ip READ ip NOTIFY ipChanged)
    Q_PROPERTY(int port READ port NOTIFY portChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QVariantList entries READ entries NOTIFY entriesChanged)
    Q_PROPERTY(QString currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(int trackStatus READ trackStatus NOTIFY trackStatusChanged)
    Q_PROPERTY(QString currentDirectory READ currentDirectory NOTIFY currentDirectoryChanged)
    Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)

public:

    static void registerQmlType();
    PlayerProxy();
    QString ip();
    int port();
    bool connected();
    QVariantList& entries();
    QString currentTrack();
    QString currentDirectory();
    int trackStatus();
    int volume();
    int similarityScore(QString , QString);

signals:

    void ipChanged();
    void portChanged();
    void connectedChanged();
    void entriesChanged();
    void currentTrackChanged();
    void trackStatusChanged();
    void currentDirectoryChanged();
    void volumeChanged();
    void errorOccured(QString msg);

public slots:

    void open(QString , int);
    void close();
    void retrieveFiles(QString);
    void retrieveCurrentTrack();
    void play(QString);
    void resume();
    void pause();
    void volumeDown();
    void volumeUp();

private:

    void messageIncome();

private:

    QList<QPair<EntryInfo , int>> findNestedLevel(QList<EntryInfo>&);
    int findNestedLevel(QString&);

    QVariantList mEntries;
    QTcpSocket mSocket;
    QString mIp;
    int mPort;
    QByteArray mBuffer;
    int mBracketsCount { 0 };
    int mLastIdx { 0 };
    bool mIgnore { false };

    QString mCurrentDirectory;
    QString mCurrentTrack;
    int mTrackStatus;
    int mVolume { 70 };

};

#endif // PLAYERPROXY_H
