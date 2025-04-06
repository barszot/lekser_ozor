#pragma once

#include "token.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>

class Lexer
{
public:
    Lexer(std::istream *source) : source(source), line(0), column(0), eofNext(false) {}
    std::istream *getSource() const
    {
        return source;
    }
    unsigned int getColumn() const
    {
        return column;
    }
    unsigned int getLine() const
    {
        return line;
    }
    bool getEofNext() const
    {
        return eofNext;
    }
    void changeSource(std::istream *newSource)
    {
        source = newSource;
        line = 0;
        column = 0;
        eofNext = false;
    }
    std::optional<Token> getToken();

private:
    std::istream *source;
    unsigned int line;
    unsigned int column;
    unsigned int maxTokenLenght = 512;
    bool eofNext;
    const char singleOperators[16] = {
        '(',
        ')',
        '}',
        '{',
        ';',
        ',',
        '.',
        '+',
        '-',
        '*',
        '/',
        '\\',
        '=',
        '?',
        '>',
        '<'};
    const std::string doubleOperators[7] = {
        ">=",
        "<=",
        "==",
        "?=",
        "=?",
        "==",
        "->"};
    const std::string polishLetters[18] = {
        "ą",
        "ć",
        "ę",
        "ł",
        "ń",
        "ó",
        "ś",
        "ź",
        "ż",
        "Ą",
        "Ć",
        "Ę",
        "Ł",
        "Ń",
        "Ó",
        "Ś",
        "Ź",
        "Ż",
    };
    bool isPolishLetter(const std::string &symbol);
    bool isDigit(const std::string &symbol);
    bool isWhiteSpace(const std::string &symbol);
    bool isOperator(const std::string &symbol);
    bool isDoubleOperator(const std::string &symbol);
    unsigned short getBytesOfSymbol(unsigned char firstChar);
    std::string currentSymbol = "";
    std::string nextSymbol = "";
    unsigned short bytesOfCurrentSymbol = 0;
    unsigned short bytesOfNextSymbol = 0;
    std::optional<std::string> getNextSymbol();
    void jumpToNextSymbol();
    const std::unordered_map<std::string, std::function<Token()>> TokenMap = {
        {"(", []()
         { return TokenOperator(TokenOperatorType::O_LEFT_PAREN); }},
        {")", []()
         { return TokenOperator(TokenOperatorType::O_RIGHT_PAREN); }},
        {"{", []()
         { return TokenOperator(TokenOperatorType::O_LEFT_BRACE); }},
        {"}", []()
         { return TokenOperator(TokenOperatorType::O_RIGHT_BRACE); }},
        {";", []()
         { return TokenOperator(TokenOperatorType::O_SEMICOLON); }},
        {",", []()
         { return TokenOperator(TokenOperatorType::O_COMMA); }},
        {".", []()
         { return TokenOperator(TokenOperatorType::O_DOT); }},
        {"+", []()
         { return TokenOperator(TokenOperatorType::O_PLUS); }},
        {"-", []()
         { return TokenOperator(TokenOperatorType::O_MINUS); }},
        {"*", []()
         { return TokenOperator(TokenOperatorType::O_ASTERISK); }},
        {"/", []()
         { return TokenOperator(TokenOperatorType::O_SLASH); }},
        {"\\", []()
         { return TokenOperator(TokenOperatorType::O_BACKSLASH); }},
        {"=", []()
         { return TokenOperator(TokenOperatorType::O_ASSIGN); }},
        {"==", []()
         { return TokenOperator(TokenOperatorType::O_EQUAL); }},
        {"?=", []()
         { return TokenOperator(TokenOperatorType::O_EQUAL); }},
        {"=?", []()
         { return TokenOperator(TokenOperatorType::O_EQUAL); }},
        {"<", []()
         { return TokenOperator(TokenOperatorType::O_LESS); }},
        {">", []()
         { return TokenOperator(TokenOperatorType::O_GREATER); }},
        {"<=", []()
         { return TokenOperator(TokenOperatorType::O_LESS_EQUAL); }},
        {">=", []()
         { return TokenOperator(TokenOperatorType::O_GREATER_EQUAL); }},
        {"->", []()
         { return TokenOperator(TokenOperatorType::O_ARROW); }},
        {"r/", []()
         { return TokenOperator(TokenOperatorType::O_MODULO); }},
        {"c/", []()
         { return TokenOperator(TokenOperatorType::O_DIV); }},
        {"całkowita", []()
         { return TokenKeyword(TokenKeywordType::K_INT_TYPE); }},
        {"zmiennoprzecinkowa", []()
         { return TokenKeyword(TokenKeywordType::K_FLOAT_TYPE); }},
        {"warunek", []()
         { return TokenKeyword(TokenKeywordType::K_BOOL_TYPE); }},
        {"tekst", []()
         { return TokenKeyword(TokenKeywordType::K_TEXT_TYPE); }},
        {"numer", []()
         { return TokenKeyword(TokenKeywordType::K_NUMBER_TYPE); }},
        {"struktura", []()
         { return TokenKeyword(TokenKeywordType::K_STRUCT_TYPE); }},
        {"lista", []()
         { return TokenKeyword(TokenKeywordType::K_LIST_TYPE); }},
        {"dowolny", []()
         { return TokenKeyword(TokenKeywordType::K_ANY_TYPE); }},
        {"nul", []()
         { return TokenKeyword(TokenKeywordType::K_NULL); }},
        {"referencja", []()
         { return TokenKeyword(TokenKeywordType::K_REF); }},
        {"stała", []()
         { return TokenKeyword(TokenKeywordType::K_CONST); }},
        {"jeśli", []()
         { return TokenKeyword(TokenKeywordType::K_IF); }},
        {"jeżeli", []()
         { return TokenKeyword(TokenKeywordType::K_IF); }},
        {"dopóki", []()
         { return TokenKeyword(TokenKeywordType::K_WHILE); }},
        {"póki", []()
         { return TokenKeyword(TokenKeywordType::K_WHILE); }},
        {"powtórz", []()
         { return TokenKeyword(TokenKeywordType::K_REPEAT); }},
        {"kolejno", []()
         { return TokenKeyword(TokenKeywordType::K_FOR); }},
        {"i", []()
         { return TokenKeyword(TokenKeywordType::K_AND); }},
        {"oraz", []()
         { return TokenKeyword(TokenKeywordType::K_AND); }},
        {"lub", []()
         { return TokenKeyword(TokenKeywordType::K_OR); }},
        {"funkcja", []()
         { return TokenKeyword(TokenKeywordType::K_FUNCTION); }},
        {"zwróć", []()
         { return TokenKeyword(TokenKeywordType::K_RETURN); }},
        {"dekoruj", []()
         { return TokenKeyword(TokenKeywordType::K_DECORATE); }},
        {"udekoruj", []()
         { return TokenKeyword(TokenKeywordType::K_DECORATE); }},
        {"wiąż", []()
         { return TokenKeyword(TokenKeywordType::K_BIND); }},
        {"powiąż", []()
         { return TokenKeyword(TokenKeywordType::K_BIND); }},
        {"zwiąż", []()
         { return TokenKeyword(TokenKeywordType::K_BIND); }},
        {"dołącz", []()
         { return TokenKeyword(TokenKeywordType::K_INCLUDE); }},
        {"połącz", []()
         { return TokenKeyword(TokenKeywordType::K_INCLUDE); }},
        {"złącz", []()
         { return TokenKeyword(TokenKeywordType::K_INCLUDE); }},
        {"prawda", []()
         { return TokenBoolValue(TokenBoolValueType::V_BOOL, true); }},
        {"fałsz", []()
         { return TokenBoolValue(TokenBoolValueType::V_BOOL, false); }}};
};
