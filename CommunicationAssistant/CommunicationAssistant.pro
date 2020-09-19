#-------------------------------------------------
#
# Project created by QtCreator 2020-07-08T14:19:07
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommunicationAssistant
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
        mainwindow.cpp \
    commonfunc.cpp \
    serverclient.cpp \
    tcpserverwindow.cpp \
    tcpserver.cpp \
    textbrowserhexorstring.cpp \
    textbrowsesend.cpp \
    tcpclientwindow.cpp \
    serialwindow.cpp \
    udpserverwindow.cpp \
    udpclientwindow.cpp \
    userdefined.cpp

HEADERS += \
        mainwindow.h \
    commonfunc.h \
    serverclient.h \
    tcpserverwindow.h \
    tcpserver.h \
    textbrowserhexorstring.h \
    textbrowsesend.h \
    tcpclientwindow.h \
    serialwindow.h \
    udpserverwindow.h \
    udpclientwindow.h \
    userdefined.h

FORMS += \
        mainwindow.ui \
    tcpserverwindow.ui \
    textbrowserhexorstring.ui \
    textbrowsesend.ui \
    tcpclientwindow.ui \
    serialwindow.ui \
    udpserverwindow.ui \
    udpclientwindow.ui \
    userdefined.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
