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
    p2ptun/JSON/cJSON.c \
    p2ptun/KCP/ikcp.c \
    p2ptun/msg2json.c \
    p2ptun/p2ptun.c \
    p2ptun/p2ptun_client.c \
    p2ptun/p2ptun_common.c \
    p2ptun/p2ptun_server.c

HEADERS  += widget.h \
    p2ptun/JSON/cJSON.h \
    p2ptun/KCP/ikcp.h \
    p2ptun/linux_udp.h \
    p2ptun/msg2json.h \
    p2ptun/p2ptun.h \
    p2ptun/p2ptun_client.h \
    p2ptun/p2ptun_common.h \
    p2ptun/p2ptun_server.h \
    p2ptun/p2ptun_session_status.h \
    global.h

FORMS    += widget.ui


INCLUDEPATH += $$PWD/p2ptun/JSON/
