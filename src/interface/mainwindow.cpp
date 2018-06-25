#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>
#include <QScopedPointer>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QStandardItem>
#include <QList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QStandardPaths>
#include "retrievelistcmd.h"
#include "listcommand.h"
#include "playcommand.h"
#include "findplayeripcmd.h"
#include "playeripresult.h"
#include "resumecommand.h"
#include "pausecommand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) ,
    mMulticastGroup(QStringLiteral("239.255.43.21"))
{
    ui->setupUi(this);
    connect(ui->actionAdd , &QAction::triggered , this , &MainWindow::addFiles);
    connect(ui->actionTemizle , &QAction::triggered , this , &MainWindow::clearTracks);
    connect(ui->playlist , &QListView::doubleClicked , this , &MainWindow::playRequired);
    connect(mMediaPlayer.data() , &QMediaPlayer::mediaStatusChanged , this , &MainWindow::mediaFinished);
    connect(&mServer , &QTcpServer::newConnection , this, &MainWindow::newConnection);


    mUdpSocket.bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress);
    mUdpSocket.joinMulticastGroup(mMulticastGroup);
    connect(&mUdpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    ui->playlist->setModel(&mModel);

    mServer.listen(QHostAddress::Any , 8000);
}

void MainWindow::clearTracks(bool enabled){
    mModel.clear();
}

void MainWindow::addFiles(bool enabled){
    Q_UNUSED(enabled)

    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Open your music files",
                            QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0),
                            "Images (*.mp3 *.MP3 *.flac)");

    if(files.empty())
        return;

    QStandardItem item;

    for(auto file : files){

        auto item = new QStandardItem(QFileInfo(file).fileName());
        item->setData(file);
        mModel.appendRow(item);
    }

    ListCommand response;

    for(auto r = 0; r < mModel.rowCount(); ++r){
        auto idx = mModel.index(r , 0);
        auto item = mModel.itemFromIndex(idx);

        response.tracks().append(item->text());
    }

    auto responseJson = response.serialize();

    for(auto socket : mSockets){
        socket->write(QJsonDocument(responseJson).toJson());
    }
}

void MainWindow::playRequired(const QModelIndex& idx){

    auto item = mModel.itemFromIndex(idx);

    mMediaPlayer->setMedia(QUrl::fromLocalFile(item->data().toString()));
    mMediaPlayer->play();

}

void MainWindow::mediaFinished(QMediaPlayer::MediaStatus status){
    if(status == QMediaPlayer::EndOfMedia){
        if(mModel.rowCount() == 0)
            return;

        auto curr = ui->playlist->currentIndex();
        if(mModel.rowCount() - 1 <= curr.row()){
            ui->playlist->setCurrentIndex(mModel.index(0  ,0));
        } else {
            ui->playlist->setCurrentIndex(mModel.index(curr.row() + 1 , 0));
        }

        playRequired(ui->playlist->currentIndex());
    }
}

void MainWindow::newConnection(){
    if(!mServer.hasPendingConnections())
        return;

    auto socket = mServer.nextPendingConnection();

    connect(socket , &QTcpSocket::readyRead , this , &MainWindow::messageIncome);

    mSockets.append(socket);
}

void MainWindow::messageIncome(){
    auto socket = static_cast<QTcpSocket*>(QObject::sender());
    auto json = QJsonDocument::fromJson(socket->readAll()).object();

    auto cmd = json["cmd"].toString();

    if(cmd == RetrieveListCmd::CMD){
        ListCommand response;

        for(auto r = 0; r < mModel.rowCount(); ++r){
            auto idx = mModel.index(r , 0);
            auto item = mModel.itemFromIndex(idx);

            response.tracks().append(item->text());
        }

        auto responseJson = response.serialize();

        socket->write(QJsonDocument(responseJson).toJson());
    } else if(cmd == PlayCommand::CMD){
        PlayCommand playCommand;

        playCommand.deserialize(json);

        for(auto r = 0; r < mModel.rowCount(); ++r){
            auto idx = mModel.index(r , 0);
            auto item = mModel.itemFromIndex(idx);

            if(item->text() == playCommand.track()){
                //mModel.set
                ui->playlist->setCurrentIndex(item->index());
                playRequired(item->index());
            }
        }
    } else if(cmd == ResumeCommand::CMD){
        if(mMediaPlayer->state() != QMediaPlayer::PlayingState)
            mMediaPlayer->play();
    } else if(cmd == PauseCommand::CMD){
        if(mMediaPlayer->state() != QMediaPlayer::PausedState)
            mMediaPlayer->pause();
    } else if(cmd == "VolumeUp"){
        int vol = mMediaPlayer->volume() + 5;
        if(vol > 100)
            vol = 100;
        mMediaPlayer->setVolume(vol);
    } else if(cmd == "VolumeDown"){
        int vol = mMediaPlayer->volume() - 5;
        if(vol < 0)
            vol = 0;
        mMediaPlayer->setVolume(vol);
    }
}

void MainWindow::processPendingDatagrams(){
    QByteArray datagram;
    while (mUdpSocket.hasPendingDatagrams()) {
        datagram.resize(int(mUdpSocket.pendingDatagramSize()));
        QHostAddress addr;
        quint16 clientPort;
        mUdpSocket.readDatagram(datagram.data(), datagram.size() , &addr , &clientPort);
        QString data = datagram.constData();
        qDebug() << data;
        qDebug() << "From " << addr.toString() << ":" << clientPort;
        auto json = QJsonDocument::fromJson(data.toUtf8()).object();
        if(json.isEmpty())
            continue;

        auto cmd = json["cmd"].toString();

        if(cmd != FindPlayerIpCmd::CMD)
            continue;

        FindPlayerIpCmd ipCmd;

        ipCmd.deserialize(json);

        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
                PlayerIpResult result;

                result.setIp(address.toString());
                result.setPort(8000);

                mUdpSocket.writeDatagram(QJsonDocument(result.serialize()).toJson() , addr , clientPort);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

