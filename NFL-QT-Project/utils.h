#ifndef DESTRUCTION_UTILS_H
#define DESTRUCTION_UTILS_H

#include <QString>
#include <QVariant>
#include <QVector>
#include <array>
#include <QTableWidgetItem>


/*
 * Checks to see if the string is a number that includes commas.
 */
bool isCommaNumber(QString data);


/*
 * Converts a QVariant to an unsigned long long. :param okay: is
 * used to determine if the conversion was successful.
 */
unsigned long long qvarToULongLong(QVariant var, bool* okay = nullptr);


/*
 * Function to load the rows from the file at :param path: into
 * the vector :param out:.
 */
bool loadRowsFromFile(std::string path, QVector<std::array<QTableWidgetItem*, 10>>& out);

#endif // DESTRUCTION_UTILS_H
