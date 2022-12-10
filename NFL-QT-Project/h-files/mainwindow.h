#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show();

private slots:

    void on_actionHome_triggered();

    void on_actionContact_Us_triggered();

    void on_actionlogin_triggered();

    void on_actionHelp_triggered();

    void on_actionShow_Original_List_triggered();

    void on_actionShow_Updated_List_triggered();

    void updateTotalCapacity();

    void on_actionLoad_New_Entries_triggered();

    void displayConference(QAction* action);

    void redisplayConferenceMenu();

private:
    Ui::MainWindow* ui;
    QHeaderView* tableHeader;
};
#endif
