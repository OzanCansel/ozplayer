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
#include "cuesheet.h"

using namespace QsLogging;

int main(int argc, char *argv[])
{
    WindowsService wService(argc , argv);

    CueSheet::loadFrom("C:/Users/ozanc/Music/Azam Ali and Loga R. Torkian - Lamentation Of Swans (2013) [FLAC]/cue.cue");

    return wService.exec();
}
