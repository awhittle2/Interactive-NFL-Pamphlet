#include <iostream>
#include <fstream>
#include "csv.h"


namespace csv {
    // Reads an entire line from a csv stream, returning the number of items read.
    // All items will be pushed to the back of the vector `output`. Ensure that
    // :param csvStream: is a binary stream with \r\n line endings that are not
    // shortened to \n for proper data parsing.
    //
    // Throws csv::UnexpectedCharacterError if it finds a special character in a
    // field that is not quoted.
    // Throws csv::UnclosedQuoteError if it reaches the end of the stream without
    // finding a closing quote.
    // Throws csv::InvalidFileModeError if the fstream is not in binary input mode.
    // Throws csv::UnexpectedEndOfStreamError if the stream ended while the
    // function was expecting more input.
    std::size_t readLine(std::istream& csvStream, std::vector<std::string>& output, const char sep) {
        // The current line we will be using to read from.
        std::string line;
        // The current item we are reading.
        std::string item = "";
        // Output count of how many items have been found on the line.
        std::size_t count = 0;
        // Bool that tells if we are in a quoted entry. This is used when there are
        // quotation marks or commas in the entry.
        bool quoted = false;
        // Bool used when we have found a quotation mark inside of a quoted entry.
        bool foundQuote = false;
        // Bool used for when we find a \r outside of a quoted entry. We throw an
        // exception if we can't find a \n immediately afterwards.
        bool foundCR = false;
        // Bool used to tell us if the line is over.
        bool lineDone = false;

        char character;


        if (csvStream.peek() != EOF) {
            // Go through each character and figure out what to do with it.
            do {
                character = csvStream.get();

                // We need to quickly make sure we handle the last character being \r
                // outside of a quoted entry and the next one not being \n. If this
                // happens then we have invalid input.
                if (foundCR && character != '\n') {
                    throw UnexpectedCharacterError("Expected a '\\n' after the '\r' in an unquoted string, but got\"" + std::to_string(character) + "\"instead.");
                }

                switch (character) {
                case '"':
                    // If the first character was a quote, then another quote may be
                    if (quoted) {
                        if (foundQuote) {
                            foundQuote = false;
                            item += '"';
                        }
                        else {
                            foundQuote = true;
                        }
                    }
                    else if (item.length() == 0) { // If we are reading the first character and it's a quote, then 
                        quoted = true;
                    }
                    else { // Based on the standard, this is invalid, so we throw an exception.
                        throw UnexpectedCharacterError("Got unexpected quotation mark.");
                    }
                    break;
                case '\r':
                    if (quoted) {
                        item += '\r';
                    }
                    else {
                        foundCR = true;
                    }
                    break;
                case '\n':
                    if (quoted) {
                        item += '\n';
                    }
                    else {
                        foundCR = false;
                        lineDone = true;
                    }
                    break;
                default:
                    if (character == sep) {
                        if (quoted) {
                            if (foundQuote) {
                                foundQuote = false;
                                quoted = false;
                                output.push_back(item);
                                item = "";
                                count++;
                            }
                            else {
                                item += sep;
                            }
                        }
                        else {
                            output.push_back(item);
                            item = "";
                            count++;
                        }
                    }
                    else {
                        item += character;
                    }
                    break;
                }
            } while(csvStream.peek() != EOF && !lineDone);
            // If a quote is still open by the end of it, then we have malformed input.
            if (quoted && !foundQuote) {
                throw UnclosedQuoteError("Found a quote that was opened by never closed.");
            }
            // If the last character was a '\r' and the stream ended, throw an exception.
            if (foundCR) {
                throw UnexpectedEndOfStreamError("Stream ended while waiting for a '\\n' to match the '\\r'.");
            }
            output.push_back(item);
            count++;
        }
        return count;
    }

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
    std::size_t readStream(std::istream& csvStream, std::vector<std::vector<std::string>>& output, std::size_t lineCount, bool strict) {
        std::size_t maxTokens = 0;
        std::size_t tokensRead = 0;

        // The number of lines read.
        std::size_t lines = 0;

        // Vector used if strict mode is on so we don't end up pushing anything to
        // the output vector unless we are sure the input is good.
        std::vector<std::vector<std::string>> tempOutput;

        // What we use to push_back to depending on the value of :param strict:.
        std::vector<std::vector<std::string>>& outputVector = (strict ? tempOutput : output);

        std::vector<std::string> nextLine;

        while (csvStream.peek() != EOF && (lineCount == 0 || lines < lineCount)) {
            tokensRead = readLine(csvStream, nextLine);
            outputVector.push_back(nextLine);
            nextLine.clear();
            if (lines == 0) {
                maxTokens = tokensRead;
            }
            if (strict && maxTokens != tokensRead) {
                throw std::length_error("Previously got " + std::to_string(maxTokens) + " tokens but the last line had " + std::to_string(tokensRead) + ".");
            }

            lines++;
        }

        if (strict) {
            // If strict is true and we have gotten to this point then the lines should
            // all be perfectly fine. We can just copy everything now.
            for (int i = 0; i < outputVector.size(); i++) {
                output.push_back(outputVector[i]);
            }
        }
        return maxTokens;
    }

    // Identical to csv:readStream except it takes in a file name instead of a stream.
    //
    // Throws csv::FileError if the file could not be opened.
    std::size_t readFile(const char* fileName, std::vector<std::vector<std::string>>& output, std::size_t lineCount, bool strict) {
        std::ifstream csvFile;

        // Open the file in binary read mode.
        csvFile.open(fileName, std::ios::in | std::ios::binary);

        // Check to see if the file opened.
        if (csvFile.fail()) {
            throw FileError("Could not open the file.");
        }

        // Read the file into the output vector.
        std::size_t tokens = readStream(csvFile, output, lineCount, strict);

        // Ensure we close the file.
        csvFile.close();

        // Return the maximum number of tokens per line.
        return tokens;
    }

    // Overload that makes it so that string file names are easily allowed.
    std::size_t readFile(std::string fileName, std::vector<std::vector<std::string>>& output, std::size_t lineCount, bool strict) {
        return readFile(fileName.c_str(), output, lineCount, strict);
    }

    //#### Exceptions ####//

    CSVException::CSVException(const char* msg) : std::logic_error(msg) {}
    CSVException::CSVException(const std::string& msg) : std::logic_error(msg.c_str()) {}
    FileError::FileError(const char* msg) : CSVException(msg) {}
    FileError::FileError(const std::string& msg) : CSVException(msg) {}
    UnclosedQuoteError::UnclosedQuoteError(const char* msg) : UnexpectedEndOfStreamError(msg) {}
    UnclosedQuoteError::UnclosedQuoteError(const std::string& msg) : UnexpectedEndOfStreamError(msg) {}
    UnexpectedCharacterError::UnexpectedCharacterError(const char* msg) : CSVException(msg) {}
    UnexpectedCharacterError::UnexpectedCharacterError(const std::string& msg) : CSVException(msg) {}
    UnexpectedEndOfStreamError::UnexpectedEndOfStreamError(const char* msg) : CSVException(msg) {}
    UnexpectedEndOfStreamError::UnexpectedEndOfStreamError(const std::string& msg) : CSVException(msg) {}

}
