#ifndef SERVERFINDER_H
#define SERVERFINDER_H

#include <QQuickItem>
#include <QUdpSocket>
#include <QThread>
#include <QVariantList>
#include <QTimer>

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
    void searchTimedout();

signals:

    void searchStarted();
    void searchFinished();
    void serversChanged();

private:

    QString selfAddress();
    QHostAddress mMulticastGroup;
    QUdpSocket mSocket;
    QVariantList mServers;
    QTimer mSearchTimeout;

};

#endif // SERVERFINDER_H
