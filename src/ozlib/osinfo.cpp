#include "osinfo.h"

bool OsInfo::isWindows(){
#ifdef Q_OS_WIN
    return true;;
#endif
#ifdef Q_OS_WIN32
    return true;
#endif
    return false;
}
