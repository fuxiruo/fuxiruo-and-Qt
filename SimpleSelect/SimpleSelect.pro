CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(qhsimpleselectplugin)
TEMPLATE    = lib

HEADERS     = qhsimpleselectplugin.h
SOURCES     = qhsimpleselectplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(qhsimpleselect.pri)

INCLUDEPATH += $$PWD/../includes

Debug:{
    LIBS += $$PWD/../lib/platformd.dll
    LIBS += $$PWD/../lib/handler3d.dll
}
Release:{
    LIBS += $$PWD/../lib/platform.dll
    LIBS += $$PWD/../lib/handler3.dll
}
