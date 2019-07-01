#ifndef DIALOGREGISTER_H
#define DIALOGREGISTER_H

#include <QDialog>

namespace Ui {
class DialogRegister;
}

class DialogRegister : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegister(QWidget *parent = nullptr);
    ~DialogRegister();

private slots:


    void on_Register_clicked();

    void on_Cancel_clicked();

private:
    Ui::DialogRegister *ui;
    void setBackground(QString src);
signals:
    void send(QString username,QString password);
};

#endif // DIALOGREGISTER_H
