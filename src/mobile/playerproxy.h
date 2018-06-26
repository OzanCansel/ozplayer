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

public:

    static void registerQmlType();
    PlayerProxy();
    QString ip();
    int port();
    bool connected();
    QVariantList& entries();

signals:

    void ipChanged();
    void portChanged();
    void connectedChanged();
    void entriesChanged();
    void errorOccured(QString msg);

public slots:

    void open(QString , int);
    void retrieveFiles(QString);

private:

    void messageIncome();

private:

    QList<QPair<EntryInfo , int>> findNestedLevel(QList<EntryInfo>&);
    int findNestedLevel(QString&);

    QVariantList mEntries;
    QTcpSocket mSocket;
    QString mIp;
    int mPort;

};

#endif // PLAYERPROXY_H
