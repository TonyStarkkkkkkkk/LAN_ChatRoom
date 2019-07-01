#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Login_clicked();

    void on_Register_clicked();

    void on_username_textEdited(const QString &arg1);

    void on_password_textEdited(const QString &arg1);

    void receive(QString username, QString password);
private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event);
    bool isRight(QString username,QString password);
    void initPic();
    void setBackground(QString src);
signals:
    void send(QString username, QString password);
};

#endif // MAINWINDOW_H
