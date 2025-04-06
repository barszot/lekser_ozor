#include "lexer.h"
#include "lexer_errors.h"
#include <cctype>

unsigned short Lexer::getBytesOfSymbol(unsigned char firstChar)
{

    if (firstChar < 0x80)
    {
        // Znak ASCII (1 bajt)
        return 1;
    }
    else if ((firstChar & 0xE0) == 0xC0)
    {
        // Znak 2-bajtowy (UTF-8)
        return 2;
    }
    else if ((firstChar & 0xF0) == 0xE0)
    {
        // Znak 3-bajtowy (UTF-8)
        return 3;
    }
    else if ((firstChar & 0xF8) == 0xF0)
    {
        // Znak 4-bajtowy (UTF-8)
        return 4;
    }
    else if ((firstChar & 0xFC) == 0xF8)
    {
        // Znak 5-bajtowy (UTF-8)
        return 5;
    }
    else if ((firstChar & 0xFE) == 0xFC)
    {
        // Znak 6-bajtowy (UTF-8)
        return 6;
    }
    else
    {
        throw LexerException("getBytesOfSymbol: Nieprawidłowy znak UTF-8", line, column);
    }
}

std::optional<std::string> Lexer::getNextSymbol()
{
    unsigned char firstChar;
    source->get(reinterpret_cast<char &>(firstChar));
    if (source->eof())
    {
        eofNext = true;
        return std::nullopt;
    }
    std::string symbol(1, firstChar); // tworzy string zawierający 1 znak: firstChar
    unsigned short bytesOfSymbol = getBytesOfSymbol(firstChar);
    unsigned int nextChar;
    for (int i = 1; i < bytesOfSymbol; i++)
    {

        source->get(reinterpret_cast<char &>(nextChar));
        if (source->eof())
        {
            throw LexerException("Błąd: EOF wewnątrz znaku UTF-8!", line, column);
        }
        symbol += nextChar;
    }
    source->seekg(-bytesOfSymbol, std::ios::cur);
    nextSymbol = symbol;
    return symbol;
}

void Lexer::jumpToNextSymbol()
{
    if (nextSymbol == "\r")
        column = 0;

    else if (nextSymbol == "\n")
    {
        column = 0;
        line++;
    }
    else
        column++;

    source->seekg(nextSymbol.length(), std::ios::cur);
    currentSymbol = nextSymbol;
    nextSymbol = "";
    if (source->eof())
        eofNext = true;
}

bool Lexer::isPolishLetter(const std::string &symbol)
{
    if (symbol.length() == 1)
    {
        return std::isalpha(symbol[0]);
    }
    for (int i = 0; i < 18; i++)
    {
        if (symbol == polishLetters[i])
            return true;
    }
    return false;
}
bool Lexer::isDigit(const std::string &symbol)
{
    return symbol.length() == 1 && std::isdigit(symbol[0]);
}

bool Lexer::isWhiteSpace(const std::string &symbol)
{
    return symbol.length() == 1 &&
           (symbol == "\n" || symbol == " " || symbol == "\r" || symbol == "\t");
}

bool Lexer::isOperator(const std::string &symbol)
{
    if (symbol.length() != 1)
        return false;
    for (int i = 0; i < 16; i++)
    {
        if (symbol[0] == singleOperators[i])
            return true;
    }
    return false;
}

bool Lexer::isDoubleOperator(const std::string &symbol)
{
    if (symbol.length() != 2)
        return false;
    for (int i = 0; i < 7; i++)
    {
        if (symbol == doubleOperators[i])
            return true;
    }
    return false;
}

std::optional<Token>
Lexer::getToken()
{
    if (eofNext)
        return std::nullopt;
    if (source->eof())
    {
        eofNext = true;
        return std::nullopt;
    }
    bool buildNumber = false;
    bool buildIdentifier = false;
    bool buildText = false;
    bool buildOperator = false;

    bool crossedFloatDot = false;
    bool crossedQuotaMark = false;
    bool crossedSlash = false;
    bool crossedDoubleSlash = false;
    bool crossedBigComment = false;

    double numberValue = 0;

    std::string textValue = "";

    double dotCoefficent = 1.0;

    auto emptyState = [&]() -> bool
    {
        return (!buildNumber && !buildIdentifier && !buildText && !buildOperator);
    };
    auto createToken = [&]() -> Token
    {
        if (buildNumber && !crossedFloatDot)
            return TokenIntValue(TokenIntValueType::V_INT, numberValue);
        else if (buildNumber)
            return TokenFloatValue(TokenFloatValueType::V_FLOAT, numberValue);
        else if (buildIdentifier)
        {
            auto findKey = TokenMap.find(textValue);
            if (findKey != TokenMap.end())
                return findKey->second();
            return TokenTextValue(TokenTextValueType::V_IDENTIFIER, textValue);
        }
        else if (buildText && crossedQuotaMark)
            return TokenTextValue(TokenTextValueType::V_TEXT, textValue);
        else if (buildText && (crossedDoubleSlash || crossedBigComment))
            return TokenTextValue(TokenTextValueType::V_COMMENT, textValue);
        else if (buildOperator)
        {
            auto findKey = TokenMap.find(textValue);
            if (findKey != TokenMap.end())
                return findKey->second();
            throw LexerException("Nieprawidłowy operator " + textValue, line, column);
        }
        else
            throw LexerException("Problem z funkcją createToken!", line, column);
    };
    unsigned int tokenLenght = 0;
    while (tokenLenght < maxTokenLenght)
    {

        std::optional<std::string> optCurrentSymbol = getNextSymbol();
        // std::cout << *optCurrentSymbol << '\n';
        if (!eofNext && !optCurrentSymbol)
        {
            throw LexerException("Nie udało się wczytać znaku do tokenu", line, column);
        }

        else if (eofNext)
        {
            if (!emptyState())
            {
                return createToken();
            }
            else
            {
                std::cout << "UWAGA EOF\n";
                return std::nullopt;
            }
        }
        if (*optCurrentSymbol == "\n" && crossedDoubleSlash)
        {
            jumpToNextSymbol();
            return TokenTextValue(TokenTextValueType::V_COMMENT, textValue);
        }
        if (isWhiteSpace(*optCurrentSymbol))
        {
            if (emptyState())
            {
                jumpToNextSymbol();
                continue;
            }
            if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
                continue;
            }
            return createToken();
        }
        else if (isDigit(*optCurrentSymbol))
        {
            if (emptyState())
            {
                jumpToNextSymbol();
                buildNumber = true;
                numberValue = std::stoi(*optCurrentSymbol);
            }
            else if (buildNumber && !crossedFloatDot)
            {
                jumpToNextSymbol();
                numberValue = 10 * numberValue + std::stoi(*optCurrentSymbol);
            }
            else if (buildNumber)
            {
                jumpToNextSymbol();
                dotCoefficent *= 0.1;
                numberValue += dotCoefficent * std::stoi(*optCurrentSymbol);
            }
            else if (buildIdentifier || buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else
                return createToken();

            if (buildIdentifier || buildNumber)
                tokenLenght++;
        }
        else if (*optCurrentSymbol == ".")
        {
            if (emptyState())
            {
                jumpToNextSymbol();
                buildOperator = true;
                textValue += *optCurrentSymbol;
                return createToken();
            }
            else if (buildNumber && !crossedFloatDot)
            {
                jumpToNextSymbol();
                crossedFloatDot = true;
            }
            else if (buildNumber && crossedFloatDot)
            {
                throw LexerException("Dwie kropki w liczbie! - błąd!", line, column);
            }
            else
                return createToken();
        }
        else if (*optCurrentSymbol == "\"")
        {
            if (buildText && crossedQuotaMark)
            {
                jumpToNextSymbol();
                return TokenTextValue(TokenTextValueType::V_TEXT, textValue);
            }
            if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else if (emptyState())
            {
                jumpToNextSymbol();
                buildText = true;
                crossedQuotaMark = true;
            }
            else
            {
                return createToken();
            }
        }
        else if (isPolishLetter(*optCurrentSymbol) || *optCurrentSymbol == "_")
        {
            if (emptyState())
            {
                jumpToNextSymbol();
                buildIdentifier = true;
                textValue += *optCurrentSymbol;
                tokenLenght++;
            }
            else if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else if (buildNumber)
            {
                if (*optCurrentSymbol == "c" || *optCurrentSymbol == "r")
                    return createToken();
                else
                    throw LexerException("Litera bezpośrednio po liczbie - błąd!", line, column);
            }
            else if (buildIdentifier)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
                tokenLenght++;
            }
            else if (buildOperator)
            {
                return createToken();
            }
        }
        else if (*optCurrentSymbol == "/")
        {
            if (emptyState())
            {
                jumpToNextSymbol();
                crossedSlash = true;
                buildOperator = true;
                textValue = *optCurrentSymbol;
            }
            else if (crossedSlash)
            {
                jumpToNextSymbol();
                buildOperator = false;
                buildText = true;
                crossedSlash = false;
                crossedDoubleSlash = true;
                textValue = "";
            }
            else if (buildIdentifier && (textValue == "c" || textValue == "r"))
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
                return TokenMap.find(textValue)->second();
            }
            else if (buildText && crossedQuotaMark)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else if (buildText && crossedBigComment && textValue[textValue.length() - 1] == '*')
            {
                jumpToNextSymbol();
                textValue.pop_back();
                return TokenTextValue(TokenTextValueType::V_COMMENT, textValue);
            }
            else if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else if (buildIdentifier || buildNumber)
                return createToken();

            else
                throw LexerException("Błąd ze slashem", line, column);
        }
        else if (*optCurrentSymbol == "*")
        {
            if (crossedSlash)
            {
                jumpToNextSymbol();
                buildOperator = false;
                buildText = true;
                crossedSlash = false;
                crossedBigComment = true;
                textValue = "";
            }
            else if (emptyState())
            {
                jumpToNextSymbol();
                buildOperator = true;
                textValue += *optCurrentSymbol;
                return createToken();
            }
            else if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else if (buildIdentifier || buildNumber || buildOperator)
            {
                return createToken();
            }
            else
                throw LexerException("Błąd ze asteriksem", line, column);
        }

        else if (isOperator(*optCurrentSymbol))
        {
            if (emptyState())
            {
                jumpToNextSymbol();
                buildOperator = true;
                textValue += *optCurrentSymbol;
            }
            else if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else if (buildOperator)
            {
                if (isDoubleOperator(textValue + *optCurrentSymbol))
                {
                    jumpToNextSymbol();
                    textValue += *optCurrentSymbol;
                }
                return createToken();
            }
            else
            {
                return createToken();
            }
        }
        else
        {
            if (buildText)
            {
                jumpToNextSymbol();
                textValue += *optCurrentSymbol;
            }
            else
                throw LexerException("Coś nie tak w ostatnim głównym elsie", line, column);
        }
    }
    throw LexerException("Za długa liczba / nazwa zmiennej / komentarz ", line, column);
}
