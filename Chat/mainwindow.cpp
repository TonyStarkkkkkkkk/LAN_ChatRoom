#include "chatroom.h"
#include "dialogregister.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
/*
 EditBy 向正中，19/6/28
 坑1：MSVC编译对中文支持不好，编译会出现报错；使用QStringLiteral()对中文进行处理，并且设置utf-8为带BOM
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);// 禁止最大化按钮
    setFixedSize(this->width(),this->height());//禁止该变窗口大小


    //设置背景
    setBackground(":/Pictures/background3.png");
    //设置输入框透明
    ui->username->setStyleSheet("background:transparent;border-width:1;border-style:outset");
    ui->password->setStyleSheet("background:transparent;border-width:1;border-style:outset");

    this->setWindowTitle(QStringLiteral("Chat"));//设置title
    ui->username->setPlaceholderText(QStringLiteral("        Input UserName         "));
    ui->username->setClearButtonEnabled(true);

    ui->password->setPlaceholderText(QStringLiteral("        Input Password         "));
    ui->password->setEchoMode(QLineEdit::Password);//设置为密码格式
    ui->password->setClearButtonEnabled(true);

    //设置图片
    //initPic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_clicked()//监听登录按钮
{


    QString username = ui->username->text();
    QString password = ui->password->text();
    if((!username.isEmpty())&&(!password.isEmpty())){
        //输入不为空

        if(isRight(username,password)){//连接数据库，校验密码是否正确
            QMessageBox::information(this,QStringLiteral("Tips"),"Welcome!",QMessageBox::Ok);
            //存在
            //CONNECT()
            ChatRoom* widget=new ChatRoom();
            widget->setWindowTitle("Chat");
            connect( this, SIGNAL (send(QString ,QString )),widget, SLOT(receive(QString ,QString )) );
            emit send(username,password);
            widget->show();
            this->close();
        }else{
            //不存在
            QMessageBox::information(this,QStringLiteral("Tips"),QStringLiteral("Username or Password wrong"),QMessageBox::Ok);
        }
    }else{
        QMessageBox::information(this,QStringLiteral("Tips"),QStringLiteral("Input Username or Password"),QMessageBox::Ok);
    }




}

void MainWindow::on_Register_clicked()//监听注册按钮
{
    DialogRegister dlg;
    connect ( &dlg, SIGNAL (send(QString ,QString )),this, SLOT(receive(QString ,QString )) );
    dlg.exec();
}


void MainWindow::on_username_textEdited(const QString &arg1)
{
    //输入账号时，表现为睁眼效果
    QImage *loginImage= new QImage();
    loginImage->load(":/Pictures/LoginPic3.png");
    loginImage->scaled(ui->LoginPic->size(), Qt::KeepAspectRatio);
    ui->LoginPic->setScaledContents(true);
    ui->LoginPic->setPixmap(QPixmap::fromImage(*loginImage));
}

void MainWindow::on_password_textEdited(const QString &arg1)
{
    //输入密码时，表现为闭眼效果
    QImage *loginImage= new QImage();
    loginImage->load(":/Pictures/LoginPic2.jpg");
    loginImage->scaled(ui->LoginPic->size(), Qt::KeepAspectRatio);
    ui->LoginPic->setScaledContents(true);
    ui->LoginPic->setPixmap(QPixmap::fromImage(*loginImage));
}

bool MainWindow::isRight(QString username, QString password){
    return true;
}

void MainWindow::initPic(){
    QImage *loginImage= new QImage();
    loginImage->load(":/Pictures/LoginPic3.png");
    loginImage->scaled(ui->LoginPic->size(), Qt::KeepAspectRatio);
    ui->LoginPic->setScaledContents(true);
    ui->LoginPic->setPixmap(QPixmap::fromImage(*loginImage));

//    QImage *leftImage= new QImage();
//    leftImage->load(":/Pictures/LeftPic.png");
//    leftImage->scaled(ui->LeftPic->size(), Qt::KeepAspectRatio);
//    ui->LeftPic->setScaledContents(true);
//    ui->LeftPic->setPixmap(QPixmap::fromImage(*leftImage));

//    QImage *rightImage= new QImage();
//    rightImage->load(":/Pictures/RightPic.png");
//    rightImage->scaled(ui->RightPic->size(), Qt::KeepAspectRatio);
//    ui->RightPic->setScaledContents(true);
//    ui->RightPic->setPixmap(QPixmap::fromImage(*rightImage));
}

void MainWindow::setBackground(QString src){
    QImage Image;
    Image.load(src);
    QPixmap pixmap = QPixmap::fromImage(Image);
    QPixmap fitpixmap = pixmap.scaled(785, 532, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette pal;
    pal.setBrush(QPalette::Background,QBrush(fitpixmap));
    this->setPalette(pal);
}

void MainWindow::receive(QString username, QString password){
    ui->username->setText(username);
    ui->password->setText(password);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    if( )
}

//void MainWindow::send(QString username, QString password){

//}
