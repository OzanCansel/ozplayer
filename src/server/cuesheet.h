#ifndef CUESHEET_H
#define CUESHEET_H

#include "cuesheettrackentry.h"
#include "cuesheetfileentry.h"
#include <QList>

class CueSheet
{


public:

    static CueSheet loadFrom(QString filePath);
    QList<QSharedPointer<CueSheetFileEntry>>& files();

private:

    QList<QSharedPointer<CueSheetFileEntry>> mFiles;

};

#endif // CUESHEET_H
