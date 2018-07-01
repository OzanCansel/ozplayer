#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QSharedPointer<QSettings> settings , QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) ,
    mSettings(settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->baseDirectoryTextEdit->setText(mSettings->value(QStringLiteral("baseDir")).toString());

    connect(ui->browseButton , &QPushButton::clicked , this , &MainWindow::browse);
    connect(ui->saveButton , &QPushButton::clicked , this , &MainWindow::saveSettings);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveSettings(bool enabled){
    Q_UNUSED(enabled)
    mSettings->sync();
    QMessageBox::information(this , QStringLiteral("Success !") ,
                             QStringLiteral("OzPlayer directory => %0").arg(mSettings->value("baseDir").toString()));
}

void MainWindow::browse(bool enabled){
    Q_UNUSED(enabled)

    auto dir = QFileDialog::getExistingDirectory(this , QStringLiteral("Select base directory for OzPlayer") , mSettings->value(QStringLiteral("baseDir")).toString());

    if(!QFileInfo(dir).isDir())
        return;

    mSettings->setValue(QStringLiteral("baseDir") , dir);
    ui->baseDirectoryTextEdit->setText(dir);
}

void MainWindow::installService(bool enabled){
    auto serverExecutablePath = QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("server.exe"));
    //Installing service
    mServiceController.install(serverExecutablePath);
    if(!mServiceController.isInstalled())
        QMessageBox::critical(this , QStringLiteral("Error !") , QStringLiteral("Could not install the service"));

    bindServiceStatus();
}

void MainWindow::startService(bool enabled){

}

void MainWindow::stopService(bool enabled){

}

void MainWindow::bindServiceStatus(){
    //If not installed don't check whether it is running or not
    if(!mServiceController.isInstalled()){
        ui->serviceStatusLabel->setText(QStringLiteral("Not Installed"));
    } else { //If installed, check running or not
        ui->serviceStatusLabel->setText(mServiceController.isRunning() ? QStringLiteral("Running") : QStringLiteral("Not running"));
    }
}
