#ifndef LOCATINGSERVICE_H
#define LOCATINGSERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTimer>

class LocatingService : public QObject
{
    Q_OBJECT

public:

    LocatingService(QHostAddress , int , int , QString);
    void init();

private slots:

    void processPendingDatagrams();
    void broadcast();

private:

    QString bestSimilar(QString , QStringList);
    QHostAddress mAddr;
    int mPort;
    int mTcpServerPort;
    QUdpSocket mSocket;
    QString mPcName;
    QTimer mBroadcastTimer;

};

#endif // LOCATINGSERVICE_H
