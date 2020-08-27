CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(myauthorityplugin)
TEMPLATE    = lib

HEADERS     = myauthorityplugin.h \
    myauthority.h
SOURCES     = myauthorityplugin.cpp \
    myauthority.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(myauthority.pri)
