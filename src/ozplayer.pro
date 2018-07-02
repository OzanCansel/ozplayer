TEMPLATE = subdirs

android: {
SUBDIRS += \
    ozlib \
    mobile
}

!android: {
SUBDIRS += \
    ozlib \
    mobile \
    configurator \
    server \
	third_party/qslog
}


