#ifndef CHATROOM_H
#define CHATROOM_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QWidget>
#include <qtcpsocket.h>
#include <qtoolbutton.h>

namespace Ui {
class ChatRoom;
}

enum MsgType{Msg,UsrEnter,UsrLeft};
class ChatRoom : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoom(QWidget *parent = nullptr);
    ~ChatRoom();

protected:
    void UserEnter(QString usrname,QString ipaddr);//用户加入
    void UserLeft(QString usrname,QString time);//用户离开
    void sendMsg(MsgType type);//send message by udp to others

    QString getIP();//获取ip
    QString getUsername();//获取用户名
    QString getMsg();//获取输入框消息



private slots:
    void processPendingDatagrams();

    void on_font_currentFontChanged(const QFont &f);

    void on_size_currentIndexChanged(int index);

    void RecMsg(QString Msg,QString usrName,QString time);//接收到信息

    void on_bold_clicked();

    void on_italic_clicked();

    void on_under_clicked();

    void on_clean_clicked();

    void on_Send_clicked();

    void on_Close_clicked();

    void receive(QString username, QString password);
    void on_color_clicked();

private:
    Ui::ChatRoom *ui;
    QUdpSocket *udpSocket;//Socket
    qint16 port;//port num
    void setBtnIcon(QToolButton *Btn, const QString &path_image);
    int loginnum;//在线人数
    QString username;//本地用户名
    QString password;//本地密码
    QColor color;
    bool boldclick,underclick,italicclick;//记录点击次数(单双)
};

#endif // CHATROOM_H
