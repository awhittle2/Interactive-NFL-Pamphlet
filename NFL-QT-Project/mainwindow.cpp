#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    // Hide the admin menu until login.
    this->ui->menuAdmin->menuAction()->setVisible(false);
    // Set default page to Home.
    this->ui->stackedWidget->setCurrentIndex(0);
    // Hide admin help info until login.
    this->ui->adminHelpWidget->setVisible(false);
    // Connect some misalaneous slots that could be connected through the editor.
    QObject::connect(this->ui->tableWidget, SIGNAL(displayUpdated()), this, SLOT(updateTotalCapacity()));
    QObject::connect(this->ui->menuDisplay_Conference, SIGNAL(triggered(QAction*)), this, SLOT(displayConference(QAction*)));
    QObject::connect(this->ui->tableWidget, SIGNAL(listsUpdated()), this, SLOT(redisplayConferenceMenu()));
}


MainWindow::~MainWindow()
{
    delete this->ui;
}


void MainWindow::on_actionHome_triggered()
{
    this->ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_actionContact_Us_triggered()
{
    this->ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_actionHelp_triggered()
{
    this->ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_actionlogin_triggered()
{
    LoginWindow login;
    if (login.exec() == 1)
    {
        // Show the admin menu.
        this->ui->menuAdmin->menuAction()->setVisible(true);
        // Switch the contact page to the admin version.
        this->ui->contactPageStack->setCurrentIndex(1);
        // Show the admin help on the help page.
        this->ui->adminHelpWidget->setVisible(true);
    }
}


void MainWindow::on_actionShow_Original_List_triggered()
{
    this->ui->tableWidget->showOriginalList();
}


void MainWindow::on_actionShow_Updated_List_triggered()
{
    this->ui->tableWidget->showUpdatedList();
}


void MainWindow::updateTotalCapacity()
{
    unsigned long long total = this->ui->tableWidget->getTotalCapacity();
    this->ui->totalCapacityLabel->setText(QVariant(total).toString());
}


void MainWindow::on_actionLoad_New_Entries_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a CSV file..."), QString(), tr("CSV Files (*.csv)"));
    if (filename != "")
    {
        this->ui->tableWidget->loadUpdateData(filename);
    }
}


void MainWindow::displayConference(QAction* action)
{
    // Use the text of the action as the conference to show unless the text is "All".
    if (action)
    {
        if (action->text() != "All")
        {
            this->ui->tableWidget->displayConference(action->text());
        }
        else
        {
            this->ui->tableWidget->displayConference("");
        }
    }
}


void MainWindow::redisplayConferenceMenu()
{
    QVector<QString> conferences;
    this->ui->tableWidget->getConferences(conferences);

    this->ui->menuDisplay_Conference->clear();
    if (conferences.size() < 1)
    {
        this->ui->menuDisplay_Conference->addAction("No Conferences Found");
    }
    else
    {
        this->ui->menuDisplay_Conference->addAction("All");
        for (int i = 0; i < conferences.size(); i++)
        {
            this->ui->menuDisplay_Conference->addAction(conferences[i]);
        }
    }
}

void MainWindow::show()
{
    QMainWindow::show();
    this->ui->tableWidget->loadOriginalData("NFL Information.csv");
}
