#include "mainwindow.h"
#include <QApplication>

/*
 EditBy 向正中，19/6/28
 坑1：MSVC编译对中文支持不好，编译会出现报错；使用QStringLiteral()对中文进行处理，并且设置utf-8为带BOM
 QT版本:5.12.3
 编译版本:MSVC版
 实现方式:UDP广播，所以请一定在同一个局域网运行，否则会接收消息失败。
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
