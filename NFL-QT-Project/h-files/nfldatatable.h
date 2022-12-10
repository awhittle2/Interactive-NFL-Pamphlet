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
    typedef std::array<QTableWidgetItem*, 10> ROW;

    explicit NFLDataTable(QWidget *parent = nullptr);
    void showOriginalList();
    void showUpdatedList();
    void loadOriginalList(QVector<ROW>& originalList);
    void loadOriginalData(QString path);
    void displayConference(QString conference);
    void loadUpdateData(QString path);

    void addRow(ROW& row);

    unsigned long long getTotalCapacity() const;

    void getConferences(QVector<QString>& out);
protected:
    void redisplayData();
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
    bool ascending;
    int lastColumn;
    bool onlyShowingOriginal;
};

#endif
