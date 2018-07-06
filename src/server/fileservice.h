#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class FileService : public QObject
{
    Q_OBJECT
public:
    void init();
    int port();
    void setBasePath(QString);
    QString basePath();
private slots:

    void newConnection();
    void clientDisconnected();
    void messageIncome();

private:

    QTcpServer mServer;
    QList<QTcpSocket*> mClients;
    QString mBasePath;

};

#endif // FILESERVICE_H
