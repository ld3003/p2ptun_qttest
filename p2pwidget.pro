#-------------------------------------------------
#
# Project created by QtCreator 2019-05-30T14:09:39
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = p2pwidget
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    p2ptest/JSON/cJSON.c \
    p2ptest/KCP/ikcp.c \
    p2ptest/msg2json.c \
    p2ptest/p2ptun.c \
    p2ptest/p2ptun_client.c \
    p2ptest/p2ptun_common.c \
    p2ptest/p2ptun_server.c

HEADERS  += widget.h \
    p2ptest/JSON/cJSON.h \
    p2ptest/KCP/ikcp.h \
    p2ptest/linux_udp.h \
    p2ptest/msg2json.h \
    p2ptest/p2ptun.h \
    p2ptest/p2ptun_client.h \
    p2ptest/p2ptun_common.h \
    p2ptest/p2ptun_server.h \
    p2ptest/p2ptun_session_status.h \
    global.h

FORMS    += widget.ui

SUBDIRS += \
    p2ptest/p2ptest.pro



INCLUDEPATH += $$PWD/p2ptest/JSON/
