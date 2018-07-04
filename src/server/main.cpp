#include <QCoreApplication>
#include <QStandardPaths>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QMessageLogContext>
#include <QDebug>
#include <qtservice.h>
#include "windowsservice.h"
#include "QsLog.h"

using namespace QsLogging;

int main(int argc, char *argv[])
{
    WindowsService wService(argc , argv);

    return wService.exec();
}
