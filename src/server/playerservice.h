#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

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

private:

    QTcpServer  mServer;
    QList<QTcpSocket*> mClients;
    QString mBasePath;

};

#endif // PLAYERSERVICE_H
