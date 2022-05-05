#include "utils.h"
#include "csv.h"
#include <QMessageBox>
#include <algorithm>


bool isCommaNumber(QString data)
{
    for (int i = 0; i < data.length(); i++)
    {
        if (data[i] != ',' && (data[i] < '0' || data[i] > '9'))
        {
            if (!(i == 0 && data[i] == '-'))
            {
                return false;
            }
        }
    }
    return true;
}


// Takes in a QVariant as well as a bool pointer for the status (optional) and returns an unsigned long long.
unsigned long long qvarToULongLong(QVariant var, bool* okay)
{
    unsigned long long ret = 0;
    bool * localOkay = okay;
    bool isOkayLocal = false;
    if (!localOkay)
    {
        localOkay = new bool;
        isOkayLocal = true;
    }
    *localOkay = false;

    // First let's try to just use the standard function and see if it works.
    ret = var.toULongLong(okay);

    // If it did not succesfully convert, we can try a few things.
    if (!*localOkay)
    {
        QString data = var.toString();
        // If the number is just a number with commas, then we can just create a string without the commos and
        if (isCommaNumber(data))
        {
            data.erase(std::remove(data.begin(), data.end(), QChar(',')), data.end());
            ret = QVariant(data).toULongLong(localOkay);
        }
    }

    if (isOkayLocal)
    {
        delete localOkay;
    }

    return ret;
}


bool loadRowsFromFile(std::string path, QVector<std::array<QTableWidgetItem*, 10>>& out)
{
    std::vector<std::vector<std::string>> fileData;
    std::size_t tokens = 0;

    try
    {
        // Read the CSV file in strict mode.
        tokens = csv::readFile(path, fileData, 0, true);
    }
    catch (csv::FileError e)
    {
        QMessageBox::critical(nullptr, "Error", "Could not find the specified file.");
        return false;
    }
    catch (csv::UnclosedQuoteError e)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: expected a close to the open quote found.");
        return false;
    }
    catch (csv::UnexpectedCharacterError e)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: unexpected character in file.");
        return false;
    }
    catch (csv::UnexpectedEndOfStreamError e)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: file ended when more data was expected.");
        return false;
    }
    catch (std::length_error)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: number of tokens per line do not match.");
        return false;
    }

    if (tokens != 10)
    {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString("Invalid input: All lines must have 10 entries, but only " + std::to_string(tokens) + " were found."));
        return false;
    }

    for (auto row = fileData.begin(); row != fileData.end(); row++)
    {
        out.emplace_back();
        for (int column = 0; column < 10; column++)
        {
            out.back()[column] = new QTableWidgetItem;
            out.back()[column]->setData(0, QVariant(QString::fromStdString((*row)[column])));
        }
    }

    return true;
}
