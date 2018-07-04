#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QSharedPointer>
#include <QProcess>
#include <QDebug>

bool deleteFirewallRule(QString service){
    QProcess deleteFirewall;
    deleteFirewall.start("netsh" , QStringList() << "advfirewall" << "firewall" << "delete" << "rule" << QStringLiteral("name=\"%0\"").arg(service));
    if(!deleteFirewall.waitForFinished())
        return false;
    qDebug() << deleteFirewall.readAll();
    return deleteFirewall.exitStatus() == QProcess::NormalExit;
}

bool addFirewallRule(QString service , QString executablePath){
    QProcess allowFirewall;
    allowFirewall.start("netsh" , QStringList() << "advfirewall" << "firewall" << "add" << "rule" << QStringLiteral("name=\"%0\"").arg(service)
                                            << "dir=in" << "action=allow" << QStringLiteral("program=\"%0\"").arg(executablePath)
                                            << "enable=yes");
    if(!allowFirewall.waitForFinished()){
        return false;
    }
    qDebug() << allowFirewall.readAll();
    return allowFirewall.exitStatus() == QProcess::NormalExit;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto settings = QSharedPointer<QSettings>::create(QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini") , QSettings::IniFormat);


    if(argc > 1){
        QString arg = argv[1];

        if(arg == "-d" ||
                arg == "--default"){
            settings->setValue(QStringLiteral("baseDir") , QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));
            settings->sync();
            auto serverPath = QDir::toNativeSeparators(QDir(QCoreApplication::applicationDirPath()).filePath("server.exe"));
            auto playerPath = QDir::toNativeSeparators(QDir(QCoreApplication::applicationDirPath()).filePath("ozplayer.exe"));
            deleteFirewallRule("ozplayer");
            deleteFirewallRule("ozplayerservice");
            addFirewallRule("ozplayerservice" , serverPath);
            addFirewallRule("ozplayer" , playerPath);
            //netsh advfirewall firewall add rule name="ozplayer" dir=in action=allow program="C:\ozplayer\server.exe" enable=yes
            return 0;
        }
    }

    MainWindow w(settings);
    w.show();

    return a.exec();
}
