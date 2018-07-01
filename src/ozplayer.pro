TEMPLATE = subdirs

android: {
SUBDIRS += \
    ozlib \
    mobile
}

win32: {
SUBDIRS += \
    ozlib \
    mobile \
    configurator \
    server
}

unix: {
SUBDIRS += \
    ozlib \
    mobile \
    configurator \
    server
}


