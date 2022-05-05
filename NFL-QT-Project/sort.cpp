#include "sort.h"

inline bool compareQTableWidgetItems(QTableWidgetItem* first, QTableWidgetItem* second, bool ascending)
{
    // QT will not properly sort numbers if they have commas in them (i.e. "6,700" and "100"
    // will not sort correctly) so to handle this we will do the checking for those types
    // ourself. If both are numbers, then we convert to actual values and then do the
    // comparison.
    //
    // Unfortunately it does look a bit messy because we shortcut some things.
    QVariant firstVar, secondVar;

    if (!isCommaNumber((firstVar = first->data(0)).toString()) ||
        !isCommaNumber((secondVar = second->data(0)).toString()))
    {
        return ascending ? *first < *second: *second < *first;
    }

    double result1 = qvarToULongLong(firstVar);
    double result2 = qvarToULongLong(secondVar);

    return ascending ? result1 < result2 : result2 < result1;
}


/*
 * Sorts the items in the vector, from [start, end), using the specified column.
 * Sorts in ascending order if ascending is true, otherwise descending.
 */
void sortColumn(QVector<std::array<QTableWidgetItem*, 10>>& rows, int start, int end, int column, bool ascending)
{
    for (int i = start; i < end; i++)
    {
        for (int j = i + 1; j < end; j++)
        {
            if (compareQTableWidgetItems(rows[i][column], rows[j][column], ascending))
            {
                std::swap(rows[i], rows[j]);
            }
        }
    }
}


/*
 * Checks if two QTableWidgetItems are equal (because it does not give us the
 * ability to do so natively).
 */
bool operator==(const QTableWidgetItem& first, const QTableWidgetItem& second)
{
    return !(first < second) && !(second < first);
}
