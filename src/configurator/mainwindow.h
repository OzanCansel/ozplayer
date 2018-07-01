#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include "qtservice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QSharedPointer<QSettings> settings ,QWidget *parent = 0);
    ~MainWindow();

public slots:

    void saveSettings(bool enabled);
    void browse(bool enabled);
    void installService(bool enabled);
    void startService(bool enabled);
    void stopService(bool enabled);


private:

    void bindServiceStatus();

    Ui::MainWindow *ui;
    QSharedPointer<QSettings> mSettings;
    QtServiceController mServiceController { "ozplayer" };
};

#endif // MAINWINDOW_H
