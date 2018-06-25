#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QScopedPointer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void addFiles(bool enabled);
    void clearTracks(bool enabled);
    void playRequired(const QModelIndex&);
    void newConnection();
    void messageIncome();
    void processPendingDatagrams();
    void mediaFinished(QMediaPlayer::MediaStatus);

private:

    Ui::MainWindow *ui;
    QStandardItemModel mModel;
    QScopedPointer<QMediaPlayer> mMediaPlayer { new QMediaPlayer() };
    QUdpSocket mUdpSocket;
    QHostAddress mMulticastGroup;
    QTcpServer mServer;
    QList<QTcpSocket*>  mSockets;

};

#endif // MAINWINDOW_H
