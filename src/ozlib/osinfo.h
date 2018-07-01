#ifndef OSINFO_H
#define OSINFO_H

#include <QObject>

class OsInfo : public QObject
{
    Q_OBJECT

public slots:

    bool isWindows();
    bool isUnix();
    bool isAndroid();

};

#endif // OSINFO_H
