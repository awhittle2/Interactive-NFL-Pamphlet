#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    this->ui->setupUi(this);
    this->ui->passwordBox->setFocus();
}

LoginWindow::~LoginWindow()
{
    delete this->ui;
}

void LoginWindow::on_pushButton_clicked()
{
    QString password = this->ui->passwordBox->text();
    if(password == "123")
    {
        this->accept();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Not correct, try again");
        msgBox.exec();
    }
}

