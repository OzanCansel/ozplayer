#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValueRef>
#include <QJsonArray>
#include <QNetworkInterface>
#include <QThread>
#include "retrievelistcmd.h"
#include "listcommand.h"
#include "playcommand.h"
#include "findplayeripcmd.h"
#include "playeripresult.h"
#include "pausecommand.h"
#include "resumecommand.h"
#include "namedcommand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) ,
    mMulticastGroup(QStringLiteral("239.255.43.21"))
{
    ui->setupUi(this);

    connect(mSocket.data() , &QTcpSocket::readyRead , this , &MainWindow::messageIncome);
    connect(ui->trackList , &QListView::doubleClicked , this , &MainWindow::playRequired);
    connect(ui->resumePauseButton , &QPushButton::clicked , this , &MainWindow::resumePauseClicked);
    connect(ui->volumeUpButton , &QPushButton::clicked , this , &MainWindow::volumeUp);
    connect(ui->volumeDownButton , &QPushButton::clicked , this , &MainWindow::volumeDown);

    ui->trackList->setModel(&mModel);

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            mSelfAddr = address.toString();
        }
    }

    findOzPlayer();
}

void MainWindow::resumePauseClicked(bool enabled){
    if(enabled){
        PauseCommand pauseCmd;
        mSocket->write(QJsonDocument(pauseCmd.serialize()).toJson());
        ui->resumePauseButton->setText("Devam");
    } else {
        ResumeCommand resumeCmd;
        mSocket->write(QJsonDocument(resumeCmd.serialize()).toJson());
        ui->resumePauseButton->setText("Durdur");
    }
}

void MainWindow::volumeUp(bool enabled){
    Q_UNUSED(enabled)

    NamedCommand volumeUpCmd("VolumeUp");
    mSocket->write(QJsonDocument(volumeUpCmd.serialize()).toJson());
}

void MainWindow::volumeDown(bool enabled){
    Q_UNUSED(enabled)

    NamedCommand volumeUpCmd("VolumeDown");
    mSocket->write(QJsonDocument(volumeUpCmd.serialize()).toJson());
}


void MainWindow::findOzPlayer(){
    QUdpSocket socket;

    while(true){

        QByteArray datagram = QJsonDocument(FindPlayerIpCmd().serialize()).toJson();
        socket.writeDatagram(datagram , mMulticastGroup , 45454);
        socket.open(QIODevice::ReadWrite);
        if(socket.waitForReadyRead()){
            QByteArray datagram;
            datagram.resize(int(socket.pendingDatagramSize()));
            auto result = socket.readDatagram(datagram.data() , datagram.size());
            qDebug() << "Received => " << datagram;
            auto json = QJsonDocument::fromJson(datagram).object();
            if(json.isEmpty())
                continue;

            auto cmd = json["cmd"].toString();
            if(cmd == PlayerIpResult::CMD){
                PlayerIpResult result;

                result.deserialize(json);

                if(result.ip() == mSelfAddr){
                    mSocket->connectToHost(QHostAddress::LocalHost , result.port());
                } else {
                    mSocket->connectToHost(QHostAddress(result.ip()) , result.port());
                }
                if(!mSocket->waitForConnected() || !mSocket->isOpen()){
                    qDebug() << "Could not connected to host.";
                    continue;
                }

                mSocket->write(QJsonDocument(RetrieveListCmd().serialize()).toJson());
                break;
            }
        } else {
            qDebug() << "waitForReadyRead failed";
        }

        QThread::sleep(1);
    }
}

void MainWindow::messageIncome(){
    auto json = QJsonDocument::fromJson(mSocket->readAll()).object();

    auto cmd = json["cmd"].toString();
    if(cmd == ListCommand::CMD){
        mModel.clear();

        auto tracks = json["tracks"].toArray();

        for(const QJsonValueRef& track : tracks)
            mModel.appendRow(new QStandardItem(track.toString()));
    }

}

void MainWindow::playRequired(const QModelIndex& idx){
    auto item = mModel.itemFromIndex(idx);
    auto trackName = item->text();
    PlayCommand cmd;

    cmd.setTrack(trackName);

    mSocket->write(QJsonDocument(cmd.serialize()).toJson());
}

MainWindow::~MainWindow()
{
    delete ui;
}
