#ifndef OSINFO_H
#define OSINFO_H

#include <QObject>

class OsInfo : public QObject
{
    Q_OBJECT

public slots:

    bool isWindows();

};

#endif // OSINFO_H
