#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QScopedPointer>
#include <QStandardItemModel>
#include <QStandardItem>
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

    void messageIncome();
    void playRequired(const QModelIndex&);
    void findOzPlayer();
    void resumePauseClicked(bool enabled);
    void volumeUp(bool enabled);
    void volumeDown(bool enabled);

private:

    Ui::MainWindow *ui;
    QScopedPointer<QTcpSocket> mSocket { new QTcpSocket() };
    QStandardItemModel mModel;
    QHostAddress mMulticastGroup;
    QString mSelfAddr;

};

#endif // MAINWINDOW_H
