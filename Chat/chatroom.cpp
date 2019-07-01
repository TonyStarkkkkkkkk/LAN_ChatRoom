#include "chatroom.h"
#include "ui_chatroom.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QDateTime>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QColorDialog>

ChatRoom::ChatRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRoom)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);// 禁止最大化按钮
    setFixedSize(this->width(),this->height());//禁止该变窗口大小

    udpSocket = new QUdpSocket(this);
    port = 19981;
    udpSocket->bind(port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));


    loginnum=0;

    setBtnIcon(ui->clean,":/Pictures/clear.png");
    setBtnIcon(ui->bold,":/Pictures/bold.png");
    setBtnIcon(ui->italic,":/Pictures/italic.png");
    setBtnIcon(ui->under,":/Pictures/under.png");
    setBtnIcon(ui->color,":/Pictures/color.png");

    boldclick=true;
    italicclick=true;
    underclick=true;
}

ChatRoom::~ChatRoom()
{
    delete ui;
}

void ChatRoom::sendMsg(MsgType type){
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString ipaddress=getIP();
    out<<type<<getUsername();//将信息写入流

    switch (type) {
    case Msg:
        if(ui->MsgEdit->toPlainText()==""){
            QMessageBox::warning(0,tr("Warnning"),tr("Message Shouldn't be null"),QMessageBox::Ok);
            return;
        }else{
            out << ipaddress << getMsg();
            //ui->MsgBrowser->verticalScrollBar()->setValue(ui->MsgBrowser->verticalScrollBar()->maximum());
        }
        break;
    case UsrEnter :
            out << ipaddress;
            break;
    case UsrLeft :
            break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);//向端口广播
}

void ChatRoom::UserEnter(QString usrname, QString ipaddr)
{
    bool isEmpty = ui->userList->findItems(usrname, Qt::MatchExactly).isEmpty();//判断是否是相同的用户
        if (isEmpty) {
            loginnum++;
            QTableWidgetItem *user = new QTableWidgetItem(usrname);
            QTableWidgetItem *ip = new QTableWidgetItem(ipaddr);

            ui->userList->insertRow(0);
            ui->userList->setItem(0,0,user);
            ui->userList->setItem(0,1,ip);//设置用户列表

            ui->MsgBrowser->setTextColor(Qt::gray);
            ui->MsgBrowser->setCurrentFont(QFont("Times New Roman",10));
            ui->MsgBrowser->append(tr("%1 login!").arg(usrname));//刷新消息框表示新用户进入了

            ui->usernum->setText(tr("online : %1 ").arg(ui->userList->rowCount()));//更新在线人数

            sendMsg(UsrEnter);//广播告知所有客户端新用户登入
        }
}

void ChatRoom::UserLeft(QString usrname, QString time)
{
    int rowNum = ui->userList->findItems(usrname, Qt::MatchExactly).first()->row();
    ui->userList->removeRow(rowNum);
    ui->MsgBrowser->setTextColor(Qt::gray);
    ui->MsgBrowser->setCurrentFont(QFont("Times New Roman", 10));
    ui->MsgBrowser->append(tr("%1 leave at %2").arg(usrname).arg(time));
    ui->usernum->setText(tr("rest prople %1").arg(ui->userList->rowCount()));

    loginnum--;
}


QString ChatRoom::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}

QString ChatRoom::getUsername()
{
    return username;
}

QString ChatRoom::getMsg()
{
    QString msg = ui->MsgEdit->toHtml();
    ui->MsgEdit->clear();
    ui->MsgEdit->setFocus();
    return msg;
}

//void ChatRoom::processPendingDatagrams()
//{

//}

void ChatRoom::on_font_currentFontChanged(const QFont &f)
{
    ui->MsgEdit->setCurrentFont(f);
    ui->MsgEdit->setFocus();
}

void ChatRoom::on_size_currentIndexChanged(int index)
{
    ui->MsgEdit->setFontPointSize(index);
    ui->MsgEdit->setFocus();
}

void ChatRoom::RecMsg(QString Msg,QString usrName,QString time)
{
    ui->MsgBrowser->setTextColor(Qt::blue);
    ui->MsgBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->MsgBrowser->append("[ " +usrName+" ] "+ time);
    ui->MsgBrowser->append(Msg);
}



void ChatRoom::on_bold_clicked()
{
    if(boldclick){
        ui->MsgEdit->setFontWeight(QFont::Bold);
    }else{
        ui->MsgEdit->setFontWeight(QFont::Normal);
    }
    boldclick=!boldclick;

    ui->MsgEdit->setFocus();

}

void ChatRoom::on_italic_clicked()
{
    ui->MsgEdit->setFontItalic(italicclick);
    italicclick=!italicclick;
    ui->MsgEdit->setFocus();
}


void ChatRoom::on_under_clicked()
{
    ui->MsgEdit->setFontUnderline(underclick);
    underclick=!underclick;
    ui->MsgEdit->setFocus();
}

void ChatRoom::on_clean_clicked()
{
    ui->MsgBrowser->clear();//清空消息框
}

void ChatRoom::on_Send_clicked()
{
    sendMsg(Msg);
}



void ChatRoom::on_Close_clicked()
{
    sendMsg(UsrLeft);
    this->close();
}

void ChatRoom::setBtnIcon(QToolButton *Btn, const QString &path_image)
{
    Btn->setStyleSheet("QToolButton{border:Opx}");
    Btn->setIconSize(QSize(Btn->width(),Btn->height()));
    QPixmap pixmap(path_image);
    Btn->setIcon(QIcon(pixmap));
}

void ChatRoom::receive(QString username, QString password){

    //UserEnter(username,getIP());
    this->username=username;
    this->password=password;
    sendMsg(UsrEnter);
}

void ChatRoom::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int msgType;
        in >> msgType;
        QString usrName,ipAddr,msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch(msgType)
        {
        case Msg:
            in >> usrName >> ipAddr >> msg;
            ui->MsgBrowser->setTextColor(Qt::blue);
            ui->MsgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->MsgBrowser->append("[ " +usrName+" ] "+ time);
            ui->MsgBrowser->append(msg);
            break;

        case UsrEnter:
            in >>usrName >>ipAddr;
            UserEnter(usrName,ipAddr);
            break;

        case UsrLeft:
            in >>usrName;
            UserLeft(usrName,time);
            break;
        }
    }
}

void ChatRoom::on_color_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid()){
    ui->MsgEdit->setTextColor(color);
    ui->MsgEdit->setFocus();
        }
}
