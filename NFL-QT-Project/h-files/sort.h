#pragma once
#ifndef SORT_H
#define SORT_H

#include <array>
#include <QTableWidgetItem>
#include "utils.h"

void sortColumn(QVector<std::array<QTableWidgetItem*, 10>>& rows, int start, int end, int column, bool ascending);

bool operator==(const QTableWidgetItem& first, const QTableWidgetItem& second);

#endif
