#include <QCoreApplication>
#include <QStandardPaths>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <qtservice.h>
#include "windowsservice.h"

int main(int argc, char *argv[])
{
    WindowsService wService(argc , argv);

    return wService.exec();
}
