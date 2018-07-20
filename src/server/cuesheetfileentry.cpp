#include "cuesheetfileentry.h"

void CueSheetFileEntry::setMusicFile(QString file){
    mFile = file;
}

QString CueSheetFileEntry::musicFile(){
    return mFile;
}

QList<QSharedPointer<CueSheetTrackEntry>>& CueSheetFileEntry::tracks(){
    return mTracks;
}
