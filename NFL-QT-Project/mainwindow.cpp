#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QFileDialog>

// MainWindow constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Initialize the user interface
{
    // Set up the user interface
    this->ui->setupUi(this);

    // Hide the "Admin" menu
    this->ui->menuAdmin->menuAction()->setVisible(false);

    // Set the current index of the stacked widget to 0
    this->ui->stackedWidget->setCurrentIndex(0);

    // Hide the admin help widget
    this->ui->adminHelpWidget->setVisible(false);

    // Connect the "displayUpdated" signal of the table widget to the "updateTotalCapacity" slot of this class
    QObject::connect(this->ui->tableWidget, SIGNAL(displayUpdated()), this, SLOT(updateTotalCapacity()));

    // Connect the "triggered" signal of the "Display Conference" menu to the "displayConference" slot of this class
    QObject::connect(this->ui->menuDisplay_Conference, SIGNAL(triggered(QAction*)), this, SLOT(displayConference(QAction*)));

    // Connect the "listsUpdated" signal of the table widget to the "redisplayConferenceMenu" slot of this class
    QObject::connect(this->ui->tableWidget, SIGNAL(listsUpdated()), this, SLOT(redisplayConferenceMenu()));
}

// MainWindow destructor
MainWindow::~MainWindow() {
    // Delete the user interface
    delete this->ui;
}

// Slot that is called when the "Home" action is triggered
void MainWindow::on_actionHome_triggered() {
    // Set the current index of the stacked widget to 0
    this->ui->stackedWidget->setCurrentIndex(0);
}

// Slot that is called when the "Contact Us" action is triggered
void MainWindow::on_actionContact_Us_triggered() {
    // Set the current index of the stacked widget to 1
    this->ui->stackedWidget->setCurrentIndex(1);
}

// Slot that is called when the "Help" action is triggered
void MainWindow::on_actionHelp_triggered() {
    // Set the current index of the stacked widget to 2
    this->ui->stackedWidget->setCurrentIndex(2);
}

// Slot that is called when the "Login" action is triggered
void MainWindow::on_actionlogin_triggered() {
    // Create a LoginWindow object and show it
    LoginWindow login;
    if (login.exec() == 1) {
        // If the user successfully logs in, show the "Admin" menu, set the current index of the contact page stack to 1, and show the admin help widget
        this->ui->menuAdmin->menuAction()->setVisible(true);
        this->ui->contactPageStack->setCurrentIndex(1);
        this->ui->adminHelpWidget->setVisible(true);
    }
}

// Slot that is called when the "Show Original List" action is triggered
void MainWindow::on_actionShow_Original_List_triggered() {
    // Show the original list in the table widget
    this->ui->tableWidget->showOriginalList();
}

// Slot that is called when the "Show Updated List" action is triggered
void MainWindow::on_actionShow_Updated_List_triggered() {
    // Show the updated list in the table widget
    this->ui->tableWidget->showUpdatedList();
}

// Slot that updates the total capacity displayed on the main window
void MainWindow::updateTotalCapacity() {
    // Get the total capacity from the table widget
    unsigned long long total = this->ui->tableWidget->getTotalCapacity();

    // Set the text of the total capacity label to the total capacity
    this->ui->totalCapacityLabel->setText(QVariant(total).toString());
}

// Slot that is called when the "Load New Entries" action is triggered
void MainWindow::on_actionLoad_New_Entries_triggered() {
    // Show a file dialog that allows the user to select a CSV file
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a CSV file..."), QString(), tr("CSV Files (*.csv)"));

    // If a file was selected, load the update data from that file
    if (filename != "") {
        this->ui->tableWidget->loadUpdateData(filename);
    }
}

// Slot that is called when a conference menu action is triggered
void MainWindow::displayConference(QAction* action) {
    if (action) {
        // If the action's text is not "All", display the conference with the specified name
        if (action->text() != "All") {
            this->ui->tableWidget->displayConference(action->text());
        }
        // Otherwise, display all conferences
        else {
            this->ui->tableWidget->displayConference("");
        }
    }
}

// Slot that updates the "Display Conference" menu with the list of conferences
void MainWindow::redisplayConferenceMenu() {
    // Get the list of conferences from the table widget
    QVector<QString> conferences;
    this->ui->tableWidget->getConferences(conferences);

    // Clear the "Display Conference" menu
    this->ui->menuDisplay_Conference->clear();

    // If there are no conferences, add an action that says "No Conferences Found"
    if (conferences.size() < 1) {
        this->ui->menuDisplay_Conference->addAction("No Conferences Found");
    }
    // Otherwise, add an "All" action and an action for each conference
    else {
        this->ui->menuDisplay_Conference->addAction("All");
        for (int i = 0; i < conferences.size(); i++) {
            this->ui->menuDisplay_Conference->addAction(conferences[i]);
        }
    }
}

// Override of QWidget::show that is called when the main window is shown
void MainWindow::show() {
    // Call the base implementation of QWidget::show
    QMainWindow::show();

    // Load the original data from the "NFL Information.csv" file
    this->ui->tableWidget->loadOriginalData("NFL Information.csv");
}
