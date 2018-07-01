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

bool OsInfo::isUnix(){
#ifdef Q_OS_LINUX
    #ifndef Q_OS_ANDROID
        return true;
    #endif
#endif
#ifdef Q_OS_UNIX
    #ifndef Q_OS_ANDROID
        return true;
    #endif
#endif
        return false;
}

bool OsInfo::isAndroid(){
#ifdef Q_OS_ANDROID
    return true;
#endif
    return false;
}
