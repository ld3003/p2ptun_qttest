#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QUdpSocket>
//#include "global.h"
#include "p2ptun/p2ptun.h"
#include <QTimer>
#include <QDebug>
namespace Ui {
class Widget;
}
typedef struct remotestruct
{
    quint16 remote_port;
    QHostAddress remote_ip;
}REMOTE;

class Widget : public QWidget
{
    Q_OBJECT
    quint16 udp_port;
    QUdpSocket* m_pSocket;
    QTimer* m_pTimer;
    struct P2PTUN_CONN_SESSION *p2psession;

    REMOTE remote;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    static Widget* getInstance(void)
     {
        static Widget s_instance;
        return &s_instance;
     }

    void init();
    void create_udp_sock();
    int __p2pdataArrived_Fun(unsigned char *data, int len);
    int __senddata_func(unsigned char *data, int len, char pkgtype);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void udp_received();
    void sl_timeout();
    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
