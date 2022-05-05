#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // DIALOG_H
