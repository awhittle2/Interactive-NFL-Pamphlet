#ifndef NFLDATATABLE_H
#define NFLDATATABLE_H

#include <QWidget>
#include <QTableWidget>
#include <array>
#include <QVector>



class NFLDataTable : public QTableWidget
{
    Q_OBJECT
public:
    // Typedef this here so we don't have to type all of this every time.
    typedef std::array<QTableWidgetItem*, 10> ROW;

    explicit NFLDataTable(QWidget *parent = nullptr);
    void showOriginalList();
    void showUpdatedList();
    // Loads the original list. Will create a shallow copy of the vector.
    void loadOriginalList(QVector<ROW>& originalList);
    void loadOriginalData(QString path);
    void displayConference(QString conference);
    // Load the file at :param path: and refresh the update list with
    // the new entries.
    void loadUpdateData(QString path);

    void addRow(ROW& row);

    // Function to get the total capacity of all stadiums in the list.
    unsigned long long getTotalCapacity() const;

    // Store the list of conferences inside of :param out: after clearing it.
    void getConferences(QVector<QString>& out);

protected:
    // Protected function to redisplay the data in the table, using displayVector
    // as the data to display in that order.
    void redisplayData();
    // Redisplays the data in the table, using the same sort order.
    void redisplaySorted();

public Q_SLOTS:
    void sort(int column);

signals:
    void displayUpdated();
    void listsUpdated();

private:
    QVector<ROW> originalList;
    QVector<ROW> updates;
    QVector<ROW> displayData;
    bool originalLoaded;
    // Whether the sorting is ascending or not.
    bool ascending;
    // The last column that was sorted on.
    int lastColumn;
    bool onlyShowingOriginal;
};

#endif // NFLDATATABLE_H
