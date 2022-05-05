#pragma once
#ifndef SORT_H
#define SORT_H

#include <array>
#include <QTableWidgetItem>
#include "utils.h"


/*
 * Sorts the items in the vector, from [start, end), using the specified column.
 * Sorts in ascending order if ascending is true, otherwise descending.
 */
void sortColumn(QVector<std::array<QTableWidgetItem*, 10>>& rows, int start, int end, int column, bool ascending);


/*
 * Checks if two QTableWidgetItems are equal (because it does not give us the
 * ability to do so natively).
 */
bool operator==(const QTableWidgetItem& first, const QTableWidgetItem& second);

#endif // SORT_H
