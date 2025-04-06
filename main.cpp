#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "lexer.h"
#include "token_printer.h"

int main()
{
    std::ifstream file("plik.txt");
    Lexer lexer(&file);
    while (!lexer.getEofNext())
    {
        std::optional<Token> optToken = lexer.getToken();
        unsigned int line = lexer.getLine() + 1;
        unsigned int column = lexer.getColumn() + 1;
        if (optToken)
        {
            std::cout << "Linia: " << line << ", kolumna: " << column << " znak: " << *optToken << std::endl;
        }
    }
    file.close();
    return 0;
}