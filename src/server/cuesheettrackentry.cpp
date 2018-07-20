#include "CueSheetTrackEntry.h"

QString CueSheetTrackEntry::trackName(){
    return mTrackname;
}

QString CueSheetTrackEntry::displayName(){
    QString displayStr;
    if(mTrackNumber)
        displayStr.sprintf("%02d - " , mTrackNumber);
    return QString("%0%1")
            .arg(mTrackNumber ? displayStr : "")
            .arg(mTitle);
}

QString CueSheetTrackEntry::title(){
    return mTitle;
}

QString CueSheetTrackEntry::performer(){
    return mPerformer;
}

QList<Index>& CueSheetTrackEntry::startIndexes(){
    return mIndexes;
}

Index& CueSheetTrackEntry::finishIndex(){
    return mFinisIndex;
}

int CueSheetTrackEntry::trackNumber(){
    return mTrackNumber;
}

void CueSheetTrackEntry::setFinishIndex(Index idx){
    mFinisIndex = idx;
}

void CueSheetTrackEntry::setTrackName(QString trackName){
    mTrackname = trackName;
}

void CueSheetTrackEntry::setTitle(QString title){
    mTitle = title;
}

void CueSheetTrackEntry::setPerformer(QString performer){
    mPerformer = performer;
}

void CueSheetTrackEntry::setTrackNumber(int val){
    mTrackNumber = val;
}
