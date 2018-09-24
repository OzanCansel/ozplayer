#ifndef CUESHEETFILEENTRY_H
#define CUESHEETFILEENTRY_H

#include "cuesheettrackentry.h"
#include <QSharedPointer>

class CueSheetFileEntry
{

public:

    QString musicFile();
    void setMusicFile(QString);
    QList<QSharedPointer<CueSheetTrackEntry>>& tracks();
    QSharedPointer<CueSheetTrackEntry> getTrackByPos(long);

private:

    QString mFile;
    QList<QSharedPointer<CueSheetTrackEntry>> mTracks;

};

#endif // CUESHEETFILEENTRY_H
