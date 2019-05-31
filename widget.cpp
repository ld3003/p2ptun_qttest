#include "widget.h"
#include "ui_widget.h"
#define SERVER_LOCAL_BIND_PORT 10087
#define CLIENT_LOCAL_BIND_PORT 10078
#define P2PTUNSRV_ADDR "47.93.103.232"
#define P2PTUNSRV_PORT_MSG 29001
#define P2PTUNSRV_PORT_ECHO1 (P2PTUNSRV_PORT_MSG + 1)
#define P2PTUNSRV_PORT_ECHO2 (P2PTUNSRV_PORT_MSG + 2)


extern "C" int c_senddata_func(unsigned char *data, int len, char pkgtype)
{
    Widget::getInstance()->__senddata_func(data,len,pkgtype);
    return 0;
}
extern "C" int c_p2pdataArrived_Fun(unsigned char *data, int length)
{
    Widget::getInstance()->__p2pdataArrived_Fun(data,length);
    return 0;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}
int Widget::__p2pdataArrived_Fun(unsigned char *data, int len)
{
    ui->textEdit->append("p2pdataArrived_Fun !!!\n");
    return 0;
}
int Widget::__senddata_func(unsigned char *data, int len, char pkgtype)
{

    //struct sockaddr_in addr;
    //bzero(&addr, sizeof(addr));
    QString str;
    // m_pSocket->wri
    switch (pkgtype)
    {
    case P2PTUN_UDPPKG_TYPE_PING1: //获取公网AP-1包
        /*
        addr.sin_family = AF_INET;
        addr.sin_port = htons(P2PTUNSRV_PORT_ECHO1);
        addr.sin_addr.s_addr = inet_addr(P2PTUNSRV_ADDR);
        */
        remote.remote_ip=QHostAddress(P2PTUNSRV_ADDR);
        remote.remote_port=P2PTUNSRV_PORT_ECHO1;
        break;
    case P2PTUN_UDPPKG_TYPE_PING2: //获取公网AP-2包 ，第二次主要用于探测是否处于对称网络
        /*
        addr.sin_family = AF_INET;
        addr.sin_port = htons(P2PTUNSRV_PORT_ECHO2);
        addr.sin_addr.s_addr = inet_addr(P2PTUNSRV_ADDR);
        */
        remote.remote_ip=QHostAddress(P2PTUNSRV_ADDR);
        remote.remote_port=P2PTUNSRV_PORT_ECHO2;
        break;
    case P2PTUN_UDPPKG_TYPE_P2PRAWDATA:
    case P2PTUN_UDPPKG_TYPE_P2PMSG: //UDP MESSAGE 包 ，主要用于 P2P 之间的信令交互
        /*
        addr.sin_family = AF_INET;
        addr.sin_port = htons(p2psession->remote_port);
        addr.sin_addr.s_addr = inet_addr(p2psession->remote_ipaddr);
        */
        remote.remote_ip=QHostAddress(p2psession->remote_ipaddr);
        remote.remote_port=p2psession->remote_port;
#define USE_LOCAL_TESTING 1
#if (USE_LOCAL_TESTING == 1)
        // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        remote.remote_ip=QHostAddress("127.0.0.1");
        if (p2psession->workmode == P2PTUN_WORKMODE_CLIENT)
        {
            remote.remote_port=SERVER_LOCAL_BIND_PORT;
        }
        else
        {
            remote.remote_port=CLIENT_LOCAL_BIND_PORT;
        }
#endif

        str.sprintf("[%1] send to remote_ipaddr : %2:%3",pkgtype, p2psession->remote_ipaddr, p2psession->remote_port);

        ui->textEdit->append(str);
        break;

    case P2PTUN_UDPPKG_TYPE_RELAYMSG: //UDP MESSAGE 包 ，主要用于 服务器转发

        ui->textEdit->append(str);

        remote.remote_ip=QHostAddress(P2PTUNSRV_ADDR);
        remote.remote_port=P2PTUNSRV_PORT_MSG;
        break;
    }
    m_pSocket->writeDatagram((const char*)data,len,remote.remote_ip,remote.remote_port);
    // send_linux_udp_data(&addr, data, len);
}

void Widget::init()
{
    m_pSocket=new QUdpSocket();

    p2psession = p2ptun_alloc_session();
    p2psession->workmode = P2PTUN_WORKMODE_CLIENT; /*<定义工作模式*/
    p2psession->out_dat = c_senddata_func;		   /*<定义工作模式*/
    p2psession->out_p2pdat = c_p2pdataArrived_Fun;   /*<定义工作模式*/

    m_pTimer=new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(sl_timeout()));


}

void Widget::create_udp_sock()
{
    m_pSocket->bind(QHostAddress::Any,udp_port);
    connect(m_pSocket,SIGNAL(readyRead()),this,SLOT(udp_received()));
}

void Widget::on_pushButton_clicked()
{//服务器

    ui->textEdit->append("running in server\n");
    p2psession->workmode = P2PTUN_WORKMODE_SERVER;
    sprintf(p2psession->local_peername, "device_ser");
    udp_port = SERVER_LOCAL_BIND_PORT;

    create_udp_sock();
    m_pTimer->start(1000);
}

void Widget::on_pushButton_2_clicked()
{//客户端
    ui->textEdit->append("running in client\n");
    p2psession->workmode = P2PTUN_WORKMODE_CLIENT;
    sprintf(p2psession->remote_peername, "device_ser");
    sprintf(p2psession->local_peername, "device_cli");
    udp_port = CLIENT_LOCAL_BIND_PORT;

    create_udp_sock();
    m_pTimer->start(1000);
}

void Widget::udp_received()
{
    while(m_pSocket->hasPendingDatagrams())
    {
        qint32 nSize = m_pSocket->pendingDatagramSize();
        char* buf = new char[nSize];
        QHostAddress host;
        quint16 port;
        m_pSocket->readDatagram(buf,nSize,&host,&port); //读取数据，为了简单一次性读完。
        QString msg = QString("from:%1:%2 %3").arg(host.toString()).arg(port).arg(buf);
        ui->textEdit->append(msg);

        p2ptun_input_data(p2psession,(unsigned char*) buf, nSize);
        //   emit sig_recvMsg(msg);

        free(buf);

        //  m_pSocket->writeDatagram(QString("recv msg").toUtf8().data(),host,port);
    }
}

void Widget::sl_timeout()
{
    p2ptun_input_timer(p2psession);
}

void Widget::on_pushButton_3_clicked()
{
    QString str=ui->lineEdit->text();
    QByteArray b=str.toLocal8Bit();
    p2ptun_input_p2pdata(p2psession,(unsigned char*) b.data(), b.length());
}
