#pragma once
#ifndef __DESTRUCTION_CSV_H__
#define __DESTRUCTION_CSV_H__

#include <vector>
#include <string>
#include <stdexcept>

namespace csv
{
    std::size_t readLine(std::istream& csvStream, std::vector<std::string>& output, const char sep = ',');
    std::size_t readStream(std::istream& csvStream, std::vector<std::vector<std::string>>& output, std::size_t lineCount = 0, bool strict = false);
    
    std::size_t readFile(std::string fileName, std::vector<std::vector<std::string>>& output, std::size_t lineCount = 0, bool strict = false);
    std::size_t readFile(const char* fileName, std::vector<std::vector<std::string>>& output, std::size_t lineCount = 0, bool strict = false);

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
