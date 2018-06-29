#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QSharedPointer>

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
            return 0;
        }
    }

    MainWindow w(settings);
    w.show();

    return a.exec();
}
