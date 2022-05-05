#include <QObject>
#include "csv.h"
#include "nfldatatable.h"
#include "sort.h"
#include <QHeaderView>


NFLDataTable::NFLDataTable(QWidget *parent) : QTableWidget(parent)
{
    // Setup the table so the user can't modify all of the cells.
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->ascending = true;
    this->onlyShowingOriginal = false;
    this->lastColumn = -1;
    this->originalLoaded = false;

    this->horizontalHeader()->setSortIndicatorShown(true);

    // Set it up so that clicking on a column will call the sort function.
    QObject::connect(reinterpret_cast<QObject*>(this->horizontalHeader()), SIGNAL(sectionClicked(int)), this, SLOT(sort(int)));
}


void NFLDataTable::addRow(ROW& row)
{
    ROW& newRow = this->updates.emplace_back();

    for (int i = 0; i < 10; i++)
    {
        newRow[i] = row[i];
    }

    emit listsUpdated();

    if (!this->onlyShowingOriginal)
    {
        this->showUpdatedList();
    }
}


unsigned long long NFLDataTable::getTotalCapacity() const
{
    QVector<QString> foundStadiums;
    QString stadium;
    unsigned long long total = 0;

    for (auto rowIt = this->displayData.begin(); rowIt != this->displayData.end(); rowIt++)
    {
        // Use `*rowit` to actually get the row itself before trying to index it for the stadium or the capacity.
        // This line gets the row, gets the stadium name from the row, gets the data from that space, then gets the string for that data.
        stadium = (*rowIt)[1]->data(0).toString();

        if (!foundStadiums.contains(stadium))
        {
            total += qvarToULongLong((*rowIt)[2]->data(0));
            foundStadiums.push_back(stadium);
        }
    }

    return total;
}


void NFLDataTable::getConferences(QVector<QString> &out)
{
    out.clear();

    QString name;

    // Add all items from the original list that are of the correct conference.
    for (int i = 0; i < this->originalList.size(); i++)
    {
        name = this->originalList[i][5]->data(0).toString();
        if (!out.contains(name))
        {
            out.push_back(name);
        }
    }

    // Only do this part if we are not displaying the original list only.
    if (!this->onlyShowingOriginal)
    {
        // Add all items from the updates list that are of the correct conference.
        for (int i = 0; i < this->updates.size(); i++)
        {
            name = this->updates[i][5]->data(0).toString();
            if (!out.contains(name))
            {
                out.push_back(name);
            }
        }
    }

    // Sort the results.
    std::sort(out.begin(), out.end());
}


void NFLDataTable::redisplayData()
{
    if (this->rowCount() != this->displayData.size())
    {
        this->setRowCount(this->displayData.size());
    }
    for (int row = 0; row < this->rowCount(); row++)
    {
        for (int col = 0; col < 10; col++)
        {
            // We specifically give a copy so that our data won't be deleted. QT just doesn't
            // understand the concept of "I didn't make this data, it's not my job to delete
            // it" and is deleting things when they may still have a use. It also physically
            // prevents you from adding one of the items to a table after it has been added
            // to one already, so this has a secondary benefit of allowing us to place things
            // easily with the setItem function.
            this->setItem(row, col, new QTableWidgetItem(*this->displayData[row][col]));
        }
    }
    emit displayUpdated();
}


void NFLDataTable::sort(int column)
{
    bool first = true;
    // This variable is used to check for ties in the last column.
    int lastCheckedColumn = -1;
    int tieFound = false;
    int tieStart = 0;
    QTableWidgetItem* lastItem = nullptr;

    while (true)
    {
        if (first)
        {
            if (this->lastColumn != column)
            {
                this->ascending = true;
            }
            this->lastColumn = column;

            // Sort the data and swap the ascending value at the same time.
            sortColumn(this->displayData, 0, this->displayData.size(), column, (this->ascending = !this->ascending));
            this->horizontalHeader()->setSortIndicator(column, this->ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
        }
        else
        {
            for (int i = 0; i < this->displayData.size(); i++)
            {
                // If the item exists then we need to check it, otherwise we are
                // looking at the first location and can't do a comparison.
                if (lastItem)
                {
                    // If they are equal, then see if we already found a tie and
                    // continue it.
                    if (*this->displayData[i][lastCheckedColumn] == *lastItem)
                    {
                        if (!tieFound)
                        {
                            tieFound = true;
                            tieStart = i - 1;
                        }
                    }
                    else
                    {
                        // If we found a tie and no longer found one, then sort
                        // that data.
                        if (tieFound)
                        {
                            sortColumn(this->displayData, tieStart, i, column, this->ascending);
                            tieFound = false;
                        }
                    }
                }
                lastItem = this->displayData[i][lastCheckedColumn];
            }
            if (tieFound)
            {
                sortColumn(this->displayData, tieStart, this->displayData.size(), column, this->ascending);
                tieFound = false;
            }
            lastItem = nullptr;
        }

        first = false;
        lastCheckedColumn = column;

        if (column == 4)
        {
            column = 3;
        }
        else if (column == 0)
        {
            break;
        }
        else
        {
            column = 0;
        }
    }
    this->redisplayData();
}


void NFLDataTable::redisplaySorted()
{
    // If the data was sorted before, swap the value of ascending and call
    // sort, which will swap it back before doing the sorting, ensuring
    // that it sorts identically to before. Otherwise just redisplay.
    if (this->lastColumn > -1)
    {
        this->ascending = !this->ascending;
        this->sort(this->lastColumn);
    }
    else
    {
        this->redisplayData();
    }
}


void NFLDataTable::loadUpdateData(QString path)
{
    QVector<ROW> readEntries;
    loadRowsFromFile(path.toStdString(), readEntries);
    bool notFound = true;

    for (auto it = readEntries.begin(); it != readEntries.end(); it++)
    {
        // Check first to see if the entry is in the originalList.
        for (auto checkIt = this->originalList.begin(); checkIt != this->originalList.end(); checkIt++)
        {
            // Check the team names.
            if ((*it)[0]->data(0) == (*checkIt)[0]->data(0))
            {
                notFound = false;
                break;
            }
        }

        // If the entry was not in the original list, check if it is in
        // the updates list already.
        if (notFound)
        {
            for (auto checkIt = this->updates.begin(); checkIt != this->updates.end(); checkIt++)
            {
                // Check the team names.
                if ((*it)[0]->data(0) == (*checkIt)[0]->data(0))
                {
                    notFound = false;
                    break;
                }
            }

            // If the entry isn't already in the list, then add it to the updates.
            if (notFound)
            {
                this->updates.push_back(*it);
            }
        }
        notFound = true;
    }
    emit listsUpdated();

    if (!this->onlyShowingOriginal)
    {
        this->showUpdatedList();
    }
}


void NFLDataTable::displayConference(QString conference)
{
    // If the conference is an empty string, display everything.
    if (conference == "")
    {
        if (this->onlyShowingOriginal)
        {
            this->showOriginalList();
        }
        else
        {
            this->showUpdatedList();
        }
        return;
    }

    // Clear the array and prepare for data to be inserted into it.
    this->displayData.clear();
    if (this->displayData.capacity() < this->originalList.size() + this->updates.size())
    {
        this->displayData.reserve(this->originalList.size() + this->updates.size());
    }

    // Add all items from the original list that are of the correct conference.
    for (int index = 0; index < this->originalList.size(); index++)
    {
        if (this->originalList[index][5]->data(0).toString() == conference)
        {
            this->displayData.push_back(this->originalList[index]);
        }
    }

    // Only do this part if we are not displaying the original list only.
    if (!this->onlyShowingOriginal)
    {
        // Add all items from the updates list that are of the correct conference.
        for (int index = 0; index < this->updates.size(); index++)
        {
            if (this->updates[index][5]->data(0).toString() == conference)
            {
                this->displayData.push_back(this->updates[index]);
            }
        }
    }

    this->redisplaySorted();
}


void NFLDataTable::showUpdatedList()
{
    this->displayData.clear();
    // If the displayData vector does not have the size to hold
    // the entries we are going to add, then reserve that much
    // memory.
    if (this->displayData.capacity() < this->originalList.size() + this->updates.size())
    {
        this->displayData.reserve(this->originalList.size() + this->updates.size());
    }

    // Load the data from the original list.
    for (auto it = this->originalList.begin(); it != this->originalList.end(); it++)
    {
        this->displayData.push_back(*it);
    }

    // Load the data from the updates list as well.
    for (auto it = this->updates.begin(); it != this->updates.end(); it++)
    {
        this->displayData.push_back(*it);
    }

    this->redisplaySorted();
    this->onlyShowingOriginal = false;
}


void NFLDataTable::showOriginalList()
{
    this->displayData.clear();
    // If the displayData vector does not have the size to hold
    // the entries we are going to add, then reserve that much
    // memory.
    if (this->displayData.capacity() < this->originalList.size())
    {
        this->displayData.reserve(this->originalList.size());
    }

    // Load the data from the original list.
    for (auto it = this->originalList.begin(); it != this->originalList.end(); it++)
    {
        this->displayData.push_back(*it);
    }

    this->redisplaySorted();
    this->onlyShowingOriginal = true;
}

void NFLDataTable::loadOriginalData(QString path)
{
    if (!this->originalLoaded)
    {
        loadRowsFromFile(path.toStdString(), this->originalList);
        emit listsUpdated();
        this->showUpdatedList();
    }
}


void NFLDataTable::loadOriginalList(QVector<ROW> &originalList)
{
    if (!this->originalLoaded)
    {
        this->originalList = originalList;
        emit listsUpdated();
        this->showUpdatedList();
    }
}
