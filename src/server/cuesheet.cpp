#include "cuesheet.h"
#include <QFile>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>
#include <QDebug>

CueSheet CueSheet::loadFrom(QString filePath){
    CueSheet sheet;

    QFile cueFile(filePath);
    //Return if file does not exist
    if(!cueFile.open(QIODevice::ReadOnly))
        return sheet;

    sheet.setPath(filePath);

    QTextStream ss(&cueFile);
    //Read file
    auto fileContent = ss.readAll();
    auto fileEntries = fileContent.right(fileContent.length() - fileContent.indexOf(QRegExp("FILE\\s+"))).split(QRegExp("FILE\\s+") , QString::SkipEmptyParts);
    for(auto fileEntryStr : fileEntries){
        auto fileEntry = QSharedPointer<CueSheetFileEntry>::create();

        QRegularExpression musicFileExpr("(\\\"(.+)\\\".*)");
        auto musicFileMatch = musicFileExpr.match(fileEntryStr);
        if(!musicFileMatch.hasMatch())
            continue;

        //Set music file
        fileEntry->setMusicFile(QFileInfo(filePath).dir().filePath(musicFileMatch.captured(2)));
        auto entriesList = fileEntryStr.right(fileEntryStr.length() - fileEntryStr.indexOf(QRegExp("\\s+TRACK\\s+"))).split(QRegExp("\\s+TRACK\\s+") , QString::SkipEmptyParts);

        //Iterate entries
        for(auto entryStr : entriesList){
            QRegularExpression trackExpr("\\s*(\\d{1,2})\\s*(.+)") ,
                                titleExpr("TITLE\\s+\\\"(.+)\\\"") ,
                                    performerExpr("PERFORMER\\s+\\\"(.+)\\\"") ,
                                        indexExpr("INDEX\\s+(\\d{1,2})\\s+(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");

            auto entry = QSharedPointer<CueSheetTrackEntry>::create();

            auto trackMatch = trackExpr.match(entryStr);
            if(trackMatch.hasMatch())
                entry->setTrackNumber(trackMatch.captured(1).toInt());

            auto titleMatch = titleExpr.match(entryStr);
            if(titleMatch.hasMatch())
                entry->setTitle(titleMatch.captured(1));

            auto performerMatch = performerExpr.match(entryStr);
            if(performerMatch.hasMatch())
                entry->setPerformer(performerMatch.captured(1));

            auto indexMatchIt = indexExpr.globalMatch(entryStr);

            while(indexMatchIt.hasNext()){
                auto indexMatch = indexMatchIt.next();
                if(indexMatch.capturedLength() >= 5){
                    Index index;
                    index.no = indexMatch.captured(1).toInt();
                    index.millis = ( indexMatch.captured(2).toInt() * 60 * 1000 ) + (indexMatch.captured(3).toInt() * 1000 ) + indexMatch.captured(4).toInt();
                    entry->startIndexes().append(index);
                }
            }

            fileEntry->tracks().append(entry);
        }

        sheet.files().append(fileEntry);
    }

    for(auto fileEntry : sheet.files()) {
        for(auto i = 0; i < fileEntry->tracks().length(); ++i){
            if((i + 1) < fileEntry->tracks().length()){
                auto next = fileEntry->tracks()[i+1];
                if(next->startIndexes().isEmpty())
                    continue;
                fileEntry->tracks()[i]->setFinishIndex(fileEntry->tracks()[i + 1]->startIndexes()[0]);
            }
        }
    }

    return sheet;
}

QList<QSharedPointer<CueSheetFileEntry>>& CueSheet::files(){
    return mFiles;
}

QString CueSheet::path(){
    return mPath;
}

void CueSheet::setPath(QString path) {
    mPath = path;
}
