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


