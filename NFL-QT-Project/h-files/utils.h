#ifndef DESTRUCTION_UTILS_H
#define DESTRUCTION_UTILS_H

#include <QString>
#include <QVariant>
#include <QVector>
#include <array>
#include <QTableWidgetItem>

bool isCommaNumber(QString data);

unsigned long long qvarToULongLong(QVariant var, bool* okay = nullptr);

bool loadRowsFromFile(std::string path, QVector<std::array<QTableWidgetItem*, 10>>& out);

#endif
