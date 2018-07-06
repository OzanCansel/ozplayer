#include "fileservice.h"
#include "QsLog.h"
#include "downloadfilecmd.h"
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "token.h"

using namespace QsLogging;

void FileService::init()
{
    mServer.listen();
    connect(&mServer , &QTcpServer::newConnection, this , &FileService::newConnection);

    QLOG_INFO() << "FileService initiated. Port " << port();
}

int FileService::port(){
    return static_cast<int>(mServer.serverPort());
}

void FileService::newConnection(){
    if(!mServer.hasPendingConnections())
        return;

    auto socket = mServer.nextPendingConnection();
    connect(socket , &QTcpSocket::readyRead , this , &FileService::messageIncome);
    connect(socket , &QTcpSocket::disconnected , this , &FileService::clientDisconnected);

    mClients.append(socket);
}

void FileService::clientDisconnected(){
    auto socket = static_cast<QTcpSocket*>(QObject::sender());
    //Remove from list
    mClients.removeOne(socket);
    //Delete socket
    socket->deleteLater();
}

void FileService::setBasePath(QString path){
    mBasePath = path;
}

QString FileService::basePath(){
    return mBasePath;
}

void FileService::messageIncome(){
    auto socket = static_cast<QTcpSocket*>(QObject::sender());
    auto json = QJsonDocument::fromJson(socket->readAll()).object();

    //Validate json
    if(!json.contains(QStringLiteral("cmd")))
        return;

    auto cmd = json["cmd"].toString();

    if(DownloadFileCmd::CMD == cmd){
        DownloadFileCmd downloadFile;
        downloadFile.deserialize(json);
        auto filePath = QDir(mBasePath).filePath(downloadFile.filePath());
        QFile requestedFile(filePath);
        if(!requestedFile.exists()){
            QLOG_ERROR() << "File not found" << requestedFile.fileName();
            return;
        }
        requestedFile.open(QIODevice::ReadOnly);
        auto content = requestedFile.readAll();
        QLOG_INFO() << requestedFile << " is requested. Sending...";
        socket->write(Token::FileDownloadStartToken.toUtf8());
        socket->write(content);
        socket->write(Token::FileDownloadEndToken.toUtf8());
        socket->waitForBytesWritten();
    }
}
