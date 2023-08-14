#-------------------------------------------------
#
# Project created by QtCreator 2023-07-19T13:57:43
#
#-------------------------------------------------

QT       += core gui
QT += designer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testImage
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/.. $$PWD

testPrj=$$lower($${TARGET})
#message($$testPrj)
prj=$$replace(testPrj,test,qh)
#message($$prj)

CONFIG += CFG_TEST_WITH_SRC
CFG_TEST_WITH_SRC{#直接包含.pri调试或者使用静态库，要把class QDESIGNER_WIDGET_EXPORT qhScriptMode : public qhParentWidget中的QDESIGNER_WIDGET_EXPORT先去掉
DEFINES += TEST_WITH_SRC
include($$PWD/../$${prj}.pri)
}else{
#.dll同时要放在编译后的运行目录release下，否则一启动就挂
LIBS += $$PWD/../release/$${prj}plugin.dll
}

RESOURCES += \
    pic.qrc
