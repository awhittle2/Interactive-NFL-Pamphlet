/*
QTableWidget

Indexed starting at top left with coordinate (0, 0)

void QTableWidget::setItem(int row, int column, QTableWidgetItem* item)
    Sets the item at the specified position.
*/

/*
Each row of the table has 10 QTableWidgetItem instances in it. They are meant
to be pointers, so we should have a list of 10 pointers to represent a row. We
then need a dynamic array to store a list of each row. Since each row is an
array of 10 pointers, a single row would be `QTableWidget* row[10]`. Now, we
want to store these in a dynamic array, so we would want a vector.
Unfortunately, a vector does not (at least not easily) support standard arrays
as the type, so instead we can use the `array` type (std::array) which is a
template class that takes the type and the size. vector's are perfectly happy
with this so if we change it to `array<QTableWidget*, 10>` then the vector looks
like this:
*/
    vector<array<QTableWidgetItem*, 10>> originalList;
/*
Now, typing `vector<array<QTableWidgetItem*, 10>>` over and over is going to get
annoying and make things long winded, so let's typedef the
array<QTableWidgetItem*, 10> part:
*/
    typedef array<QTableWidgetItem*, 10> row;
/*
This means that the name `row` refers to an array of 10 QTableWidgetItem
pointers, and std::array can be used just like a normal [] array for our
purposes. Rewriting the previous line using that typedef, we get:
*/
    vector<row> originalList;
/*
Now we have that list that can contain each row, we need to know how to insert
them into the table. We can worry about loading the data from each row later. So
if we have that vector, and a QTableWidget defined like this:
*/
    QTableWidget* table;
/*
Then we can insert all of the items from all of the rows in the vector like
this:
*/
    for (int row = 0; row < originalList.size(); row++)
    {
        for (int i = 0; i < 10; i++)
        {
            table->setItem(row, i, originalList[row][i]);
        }
    }
/*
Now, the last part assumes that originalList is already loaded with all of
the rows and has all of the data in place. It also assumes that table has the
correct number of rows. We can fix that assumption by using this function to
set the number of rows before trying to modify them:

    void QTableWidget::setRows(int numberOfRows)

Using that as well, we can get this code:
*/
    table->setRows(originalList.size());
    for (int row = 0; row < originalList.size(); row++)
    {
        for (int i = 0; i < 10; i++)
        {
            table->setItem(row, i, originalList[row][i]);
        }
    }
/*
However, this ignores that we need a separate list for the new items that will
be loaded when the user tells the program to reread the input file or a separate
file (depending on how we implement it). So, let's change our code a little bit
to have a separate vector for that. Right now our data table (the list of
variables that we have and their type) looks like this:
*/
    vector<row> originalList;
    QTableWidget* table;
/*
Since we know this array is going to be holding the data for a row, it should be
declared identically to the original list. Knowing that, this is our new data
table:
*/
    vector<row> originalList;
    vector<row> updateList;
    QTableWidget* table;
/*
Now that we have a list for the new items called `updateList`, we can now add
it's items to the table as well. The easy way would be to create a third vector,
add all the rows from each, and then just load in all from that. Unfortunately,
this would be slow and memory-inefficient, so I believe the best method would be
to just load them in order with some small additional calculations:
*/
    // Make the number of rows be the combined amount.
    table->setRows(originalList.size() + updateList.size());
    // No changes to this for loop.
    for (int row = 0; row < originalList.size(); row++)
    {
        for (int i = 0; i < 10; i++)
        {
            table->setItem(row, i, originalList[row][i]);
        }
    }
    // An additional for loop for the updateList.
    // We need to insert after the originalList so if we just add it's size to
    // the 0 based index then that will work.
    int start = originalList.size();
    for (int row = 0; row < updateList.size(); row++)
    {
        for (int i = 0; i < 10; i++)
        {
            table->setItem(row + start, i, updateList[row][i]);
        }
    }
/*
Knowing all of this, we can actually just create a function that will
generically load two lists into a table. By doing this, we can actually shortcut
a few things like loading only one list. We can load from only list by just
giving an empty vector to the function. So let's say this is how we have defined
the function:
*/
    void loadListsIntoTable(QTableWidget* table, const vector<row>& originalList, const vector<row>& updateList)
    {
        // Make the number of rows be the combined amount.
        table->setRows(originalList.size() + updateList.size());
        // No changes to this for loop.
        for (int row = 0; row < originalList.size(); row++)
        {
            for (int i = 0; i < 10; i++)
            {
                table->setItem(row, i, originalList[row][i]);
            }
        }
        // An additional for loop for the updateList.
        // We need to insert after the originalList so if we just add it's size to
        // the 0 based index then that will work.
        int start = originalList.size();
        for (int row = 0; row < updateList.size(); row++)
        {
            for (int i = 0; i < 10; i++)
            {
                table->setItem(row + start, i, updateList[row][i]);
            }
        }
    }
/*
If we have this as our data table in the function that will be calling the
loader:
*/
    const vector<row> emptyVector = vector<row>(0); // Will always be empty.
    vector<row> originalList;
    vector<row> updateList;
    QTableWidget* table;
/*
Then we can call it in 3 different ways to load both lists, load the original
list only, or load the update list only:
*/
    // Load both lists.
    loadListsIntoTable(table, originalList, updateList);
    // Load original list only.
    loadListsIntoTable(table, originalList, emptyVector);
    // Load update list only.
    loadListsIntoTable(table, emptyVector, updateList);
/*
So now we havea a function that will, once the data is loaded into the vectors,
load that data into the display table. Next we need to know how to actually load
in the data for each row into the vectors. For that we need to take a look at
this function:

    void QTableWidgetItem::setData(int role, const QVariant& value)

This function references the class that we will use to hold all of our data:
QVariant. This class is easy to initialize and convert just about any type into.
As such, we can actually just load all of our CSV data in as strings, and then
later grab the QVariant and use it to convert to a number for something like the
total capacity. But we can get to that later.

First let's read the csv file. I actually already wrote a converter that is very
powerful so we can just use that for the reading. It takes in a string vector if
you just want a single line, otherwise it takes in a vector of string vectors.
The second function also does error checking to make sure all rows have the same
number of tokens, so I would highly recommend using that. Here is an example of
using it to get the data from the file:
*/
    // At the top we need to include this:
    #include "csv.h"

    // Now that it is included, we can use it in a function:
    vector<vector<string>> fileData;

    std::size_t tokens;

    // Out path we will be using. We can also code to get the path from the
    // user.
    string filePath = "C:\\This\\Is\\Just\\An\\Example\\Path.csv";

    tokens = csv::readFile(filePath, fileData);
/*
This code will save all of the strings into into the vector. The
vector<vector<string>> is acting as a multidimensional array of strings, with
the first index being the line and the second index being the token in the line.
The return from the function is the number of tokens per line, which we need to
check to ensure is 10 for the data to be valid. The function also has 2
additional arguments: number of lines to read and strict mode. If the number of
lines to read is 0, then it will read all available lines. If strict mode is on,
then any of the lines having a different amount of tokens will cause the
function to throw the exception std::length_error and not modify the vector.

Running in strict mode and checking the number of tokens is the best way to
validate the input. As such, putting it in a try block like this is will tell
you if it ran correctly.
*/
    try
    {
        tokens = csv::readFile(filePath, fileData, 0, true);
        // If the previous line failed, it will jump to the catch block and this
        // code will be skipped.
        if (tokens != 10)
        {
            // Handling for all lines being the wrong size can be done here.
        }
    }
    catch (std::length_error e)
    {
        // Handle the error for the lines not being the right length. I can show
        // you how to find the line that had the bad number of tokens if you
        // want to show that or I can have it so that the error will tell you
        // that in it's text.
    }
    catch (csv::CSVException e)
    {
        // There are multiple possible exceptions that could be caugth here,
        // but most of them, aside from the one for a file not being able to be
        // opened, would be handled as being invalid user input.
    }
/*
Keep in mind that if an error happens there, we specifically do not want the
data in the table to be changed at all, and should immediately return out of the
function. Aside from that, we can actually now get to loading the data. Loading
the data for each QVariant is actually rather easy. Keep in mind that this is
not checking in any way that the data may already exist somewhere, just plainly
writing it.
*/
    // Given the vector declared like this:
    vector<row> myVector;

    // This loop will read each of the lines of tokens from fileData and store
    // The data into the vector properly.
    for (int line = 0; line < fileData.size(); line++)
    {
        // We add a new row and fill the data like this:
        myVector.emplace_back(); // Adds a new row.
        // Create an object for each pointer.
        for (int i = 0; i < 10; i++)
        {
            // Add the new QTableWidgetItem.
            myVector.back()[i] = new QTableWidgetItem;
            // Fill in its data.
            myVector.back()[i]->setData(0, QVariant(fileData[line][i]));
        }
    }
/*
Now for checking if the data is already in the list, you will need to iterate
over the vector you are adding to, as well as any other vectors storing data,
and compare a value that you know will be completely unique. If you find that
value in the vectors already, then you just skip that line and move to the next
one.

Now I might be forgetting something so I might change this later, but now let's
move on to sorting. We are going to be handling sorting in two different ways:
the header and buttons. The header sorting is going to be sorting based on where
someone clicks on the column name and we sort by that. That part is going to be
a small challenge because we have to deal with an issue with the designer: it
doesn't allow direct access to the signals from the table header.

Instead, we need to handle assigning a slot to the click signal manually, which
is handled like this:
*/
    QObject::connect(this->ui->tableName->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionSort(int)));
/*
Now, two of the variable names in that are actually temporary names: tableName
and sectionSort. tableName should be replaced with the name of the QTableWidget
in the designer, and sectionSort should be replaced with the name of the
function we want to call when the header is clicked. The sort function for the
header should probably be doing two things: actual sorting and changing some
kind of indicator to show the sort direction. Those can be figured out later.

As for the buttons, we need some kind of way, probably in a menu of some kind,
to allow the user to show the original list, the update list, or the combined
list. We also need a way to bring up the contact page (I have the rest of the
code for that figured out) and the information about total seating capacity.

Below are two functions I wrote where the lower one is used so we can actually
display the numbers with commas in place so they look nice, but then still
easily convert to numbers for the total capacity later.
*/
    // Probably pretty inefficient way to check if a number is just a number with commas.
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
    unsigned long long qvarToULongLong(QVariant var, bool* okay = nullptr)
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
                QString newData = "";
                for (QChar character : data)
                {
                    if (character != ',')
                    {
                        newData += character;
                    }
                }
                ret = QVariant(newData).toULongLong(localOkay);
            }
        }

        if (isOkayLocal)
        {
            delete localOkay;
        }

        return ret;
    }


/*
SORT FUNCTION
This is a relatively inefficient way to do sorting, but it should work.
All we do is use a determinant to figure out if we want ascending or descending
and then copy out values to their correct place in the vector one at a time into
a new vector. We can then just copy back as a way to reorganize.
/*






//#### Vector explanation crap
vector<type> name;
type name[SIZE];

vector<int> myIntArray;
int myIntArray[0];

void vector<type>::push_back(type item); // Adds an item to the end of the list.
void vector<type>::pop_back(); // Removes the last item from the list.
void vector<type>::insert(iterator position, type val); // Inserts an item into the specified position, moving all items after it.
void vector<type>::erase(iterator position); // Removes an item from the specified position, moving all items after it back one.
iterator vector<type>::begin(); // Returns an iterator that starts at the first position of the vector.
iterator vector<type>::end(); // Returns an iterator that starts at the position immediately after the last item.
std::size_t vector<type>::size();

myIntArray[0] = 2; // Works whether it is an array or vector.
myIntArray.begin()+1;

for (auto it = myIntArray.begin(); it != myIntArray.end(); it++)
{
    myIntArray[it]; // Incorrect.
    *it; // Correct;
}
/*
1 3 4 5 6 7 8
1 3 4 5 6 7 8 0
1 3 4 5 6 7 8 8
1 3 4 5 6 7 7 8
...
1 3 3 4 5 6 7 8
1 2 3 4 5 6 7 8

*/


// This is something that mimics a vector.
class IntVector
{
private:
    std::size_t size; // Number of items in the vector.
    std::size_t capacity; // How many items it can possibly hold.
    const static std::size_t STEP = 5; // Number of items to extend the array by when it reaces max capacity.
    int* dynamicArray;
}

IntVector::IntVector()
{
    this->size = 0; // Number of items in the vector.
    this->capacity = 0; // How many items it can possibly hold.

    this->dynamicArray = nullptr;
}

void IntVector::push_back(int& newItem)
{
    if (this->size + 1 > this->capacity)
    {
        if (this->dynamicArray != nullptr)
        {
            int* tempArray = this->dynamicArray;
            this->dynamicArray = new int[this->capacity + STEP];
            for (int i = 0; i < this->size; i++)
            {
                this->dynamicArray[i] = tempArray[i];
            }
            delete[] tempArray;
            this->capacity += STEP;
        }
        else
        {
            this->dynamicArray = new int[STEP];
            this->capacity = STEP;
        }
    }
    this->dynamicArray[this->size++] = newItem;
}


int IntVector::getItem(std::size_t index)
{
    if (index < this->size)
    {
        return this->dynamicArray[index]
    }
    else
    {
        throw ArrayOutOfBoundsOrWhatever();
    }
}



typedef std::array<QTableWidgetItem*, 10> ROW;
QString myVariable = myVector[row][5]->data(0).toString();




/*
This is the function QTableWidget uses for sorting. We can use this as a
possible template when trying to make our sort function.
*/
void QTableModel::sort(int column, Qt::SortOrder order)
{
    QVector<QPair<QTableWidgetItem*, int> > sortable;
    QVector<int> unsortable;
    // Reserve enough space in both vectors for all of the rows ahead of time.
    sortable.reserve(rowCount());
    unsortable.reserve(rowCount());
    // Go through each item in the specified column and get the item for each
    // row in it. If there is an item in the row, the row is considered
    // sortable, otherwise it would have nothing to sort with, so it's
    // unsortable.
    for (int row = 0; row < rowCount(); ++row) {
        if (QTableWidgetItem *itm = item(row, column))
            sortable.append(QPair<QTableWidgetItem*,int>(itm, row));
        else
            unsortable.append(row);
    }
    // These two are functions, and it is determining in an actually rather
    // smart way which function to use for comparisons. By doing it like this
    // they can refer to either function using the same name. Which function is
    // called using this name (compare) is determined right now and not changed.
    const auto compare = (order == Qt::AscendingOrder ? &itemLessThan : &itemGreaterThan);
    std::stable_sort(sortable.begin(), sortable.end(), compare);
    QVector<QTableWidgetItem*> sorted_table(tableItems.count());
    QModelIndexList from;
    QModelIndexList to;
    const int numRows = rowCount();
    const int numColumns = columnCount();
    from.reserve(numRows * numColumns);
    to.reserve(numRows * numColumns);
    for (int i = 0; i < numRows; ++i) {
        int r = (i < sortable.count()
                 ? sortable.at(i).second
                 : unsortable.at(i - sortable.count()));
        for (int c = 0; c < numColumns; ++c) {
            sorted_table[tableIndex(i, c)] = item(r, c);
            from.append(createIndex(r, c));
            to.append(createIndex(i, c));
        }
    }
    emit layoutAboutToBeChanged({}, QAbstractItemModel::VerticalSortHint);
    tableItems = sorted_table;
    changePersistentIndexList(from, to); // ### slow
    emit layoutChanged({}, QAbstractItemModel::VerticalSortHint);
}

bool QTableModel::itemLessThan(const QPair<QTableWidgetItem*,int> &left,
                               const QPair<QTableWidgetItem*,int> &right)
{
    // Get the pointer to the QTableWidgetItem for each pair, dereference both,
    // and run the operator< function on them.
    return *(left.first) < *(right.first);
}
bool QTableModel::itemGreaterThan(const QPair<QTableWidgetItem*,int> &left,
                                  const QPair<QTableWidgetItem*,int> &right)
{
    return (*(right.first) < *(left .first));
}
