#ifndef SERVERFINDER_H
#define SERVERFINDER_H

#include <QQuickItem>
#include <QUdpSocket>
#include <QThread>
#include <QVariantList>
#include <QTimer>
#include "playeripresult.h"

class ServerFinder : public QQuickItem
{

    Q_OBJECT
    Q_PROPERTY(QVariantList servers READ servers NOTIFY serversChanged)

public:

    static void registerQmlType();
    ServerFinder();
    QVariantList servers();

public slots:

    void search();

private slots:

    void messageIncome();
    void ipReceived();
    void searchTimedout();
    void checkListenerSocket();

signals:

    void searchStarted();
    void searchFinished();
    void serversChanged();
    void debug(QString message);

private:

    bool exists(PlayerIpResult&);

    QString selfAddress();
    QHostAddress mMulticastGroup;
    QUdpSocket mSocket;
    QUdpSocket mListener;
    QVariantList mServers;
    QTimer mSearchTimeout;

};

#endif // SERVERFINDER_H
