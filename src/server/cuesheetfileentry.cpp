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

QSharedPointer<CueSheetTrackEntry> CueSheetFileEntry::getTrackByPos(long pos){
    for(auto track : tracks()){
        if(track->startIndexes().isEmpty())
            continue;
        if(track->startIndexes().at(0).millis < pos && pos < track->finishIndex().millis)
            return track;
    }

    return QSharedPointer<CueSheetTrackEntry>();
}
