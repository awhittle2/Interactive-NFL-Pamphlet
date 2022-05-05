#pragma once
#ifndef __DESTRUCTION_CSV_H__
#define __DESTRUCTION_CSV_H__

#include <vector>
#include <string>
#include <stdexcept>

namespace csv
{

    //#### Function Declarations ####//

    // Reads an entire line from a csv stream, returning the number of items read.
    // All items will be pushed to the back of the vector `output`. Ensure that
    // :param csvStream: is a binary stream with \r\n line endings that are not
    // shortened to \n for proper data parsing.
    //
    // Throws csv::UnexpectedCharacterError if it finds a special character in a
    // field that is not quoted.
    // Throws csv::UnclosedQuoteError if it reaches the end of the stream without
    // finding a closing quote.
    // Throws csv::UnexpectedEndOfStreamError if the stream ended while the
    // function was expecting more input.
    std::size_t readLine(std::istream& csvStream, std::vector<std::string>& output, const char sep = ',');


    // Takes in a file stream as well as a vector of string vectors and uses it as
    // an output. If :param strict: is true then it will check to make sure every
    // line is exactly the same number of characters before adding any of them to
    // the vector. Vectors will be added for each line using `push_back`.
    //
    // If :param lineCount: is not 0, then the function will only read up to the
    // specified number of lines. Defaults to 0.
    //
    // Returns the highest number of entries read.
    //
    // Throws std::length_error if in strict mode and the line lengths are not equal.
    // Internally calls csv::readLine and so will throw the same exceptions.
    std::size_t readStream(std::istream& csvStream, std::vector<std::vector<std::string>>& output, std::size_t lineCount = 0, bool strict = false);


    // Identical to csv:readStream except it takes in a file name instead of a stream.
    //
    // Throws csv::FileError if the file could not be opened.
    std::size_t readFile(std::string fileName, std::vector<std::vector<std::string>>& output, std::size_t lineCount = 0, bool strict = false);
    std::size_t readFile(const char* fileName, std::vector<std::vector<std::string>>& output, std::size_t lineCount = 0, bool strict = false);



    //#### Exceptions ####//

    class CSVException : public std::logic_error
    {
    public:
        CSVException(const char* msg);
        CSVException(const std::string& msg);
    };

    class FileError: public CSVException
    {
    public:
        FileError(const char* msg);
        FileError(const std::string& msg);
    };

    class UnexpectedCharacterError : public CSVException
    {
    public:
        UnexpectedCharacterError(const char* msg);
        UnexpectedCharacterError(const std::string& msg);
    };

    class UnexpectedEndOfStreamError : public CSVException
    {
    public:
        UnexpectedEndOfStreamError(const char* msg);
        UnexpectedEndOfStreamError(const std::string& msg);
    };

    class UnclosedQuoteError : public UnexpectedEndOfStreamError
    {
    public:
        UnclosedQuoteError(const char* msg);
        UnclosedQuoteError(const std::string& msg);
    };

}

#endif
