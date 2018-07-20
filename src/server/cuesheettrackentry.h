#ifndef CUESHEETENTRY_H
#define CUESHEETENTRY_H

#include <QString>
#include <QList>

struct Index{
    int no;
    int millis;
};

class CueSheetTrackEntry
{
public:

    QString trackName();
    QString title();
    QString performer();
    QString displayName();
    int trackNumber();
    QList<Index>& startIndexes();
    Index& finishIndex();
    void setTrackName(QString);
    void setTitle(QString);
    void setPerformer(QString);
    void setFinishIndex(Index idx);
    void setTrackNumber(int);

private:

    QString mTrackname;
    QString mTitle;
    QString mPerformer;
    QList<Index> mIndexes;
    int mTrackNumber { -1 };
    Index mFinisIndex { -1 };

};

#endif // CUESHEETENTRY_H
