#ifndef WINDOWSSERVICE_H
#define WINDOWSSERVICE_H

#include <qtservice.h>
#include "playerservice.h"
#include "locatingservice.h"
#include "fileservice.h"
#include <QSharedPointer>

#define STATUS_COMMAND 10

class WindowsService : public QtService<QCoreApplication>
{

public:

    WindowsService(int argc, char **argv);
    ~WindowsService();

protected:
    void start() final;
    void stop() final;
    void pause() final;
    void resume() final;

private:

    QSharedPointer<PlayerService> mPlayerService;
    QSharedPointer<LocatingService> mLocatorService;
    QSharedPointer<FileService> mFileService;

};

#endif // WINDOWSSERVICE_H
