#pragma once
#include <exception>
#include <string>
#include <sstream>

class LexerException : public std::exception
{
public:
    LexerException(const std::string &msg, unsigned int line, unsigned int column)
        : line(line), column(column)
    {
        std::ostringstream oss;
        oss << "Błąd: " << msg << " (linia: " << line + 1 << ", kolumna: " << column + 1 << ")";
        fullMessage = oss.str();
    }

    const char *what() const noexcept override
    {
        return fullMessage.c_str();
    }

    unsigned int getLine() const { return line; }
    unsigned int getColumn() const { return column; }

private:
    unsigned int line;
    unsigned int column;
    std::string fullMessage;
};
