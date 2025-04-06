#pragma once

#include <variant>
#include <string>
#include <optional>

enum class TokenKeywordType
{
    K_INT_TYPE,
    K_FLOAT_TYPE,
    K_BOOL_TYPE,
    K_TEXT_TYPE,
    K_NUMBER_TYPE,
    K_STRUCT_TYPE,
    K_LIST_TYPE,
    K_ANY_TYPE,
    K_NULL,
    K_REF,
    K_CONST,
    K_IF,
    K_WHILE,
    K_REPEAT,
    K_FOR,
    K_AND,
    K_OR,
    K_FUNCTION,
    K_RETURN,
    K_DECORATE,
    K_BIND,
    K_INCLUDE,
};
enum class TokenIntValueType
{
    V_INT
};
enum class TokenBoolValueType
{
    V_BOOL
};
enum class TokenFloatValueType
{
    V_FLOAT
};
enum class TokenTextValueType
{
    V_TEXT,
    V_IDENTIFIER,
    V_COMMENT
};

enum class TokenOperatorType
{
    O_LEFT_PAREN,
    O_RIGHT_PAREN,
    O_LEFT_BRACE,
    O_RIGHT_BRACE,
    O_SEMICOLON,
    O_COMMA,
    O_DOT,
    O_PLUS,
    O_MINUS,
    O_ASTERISK,
    O_SLASH,
    O_BACKSLASH,
    O_ASSIGN,
    O_EQUAL,
    O_LESS,
    O_GREATER,
    O_LESS_EQUAL,
    O_GREATER_EQUAL,
    O_ARROW,
    O_MODULO,
    O_DIV
};

using TokenType = std::variant<
    TokenKeywordType,
    TokenIntValueType,
    TokenFloatValueType,
    TokenBoolValueType,
    TokenTextValueType,
    TokenOperatorType>;

using ValueOfToken = std::variant<std::monostate, std::string, int, float, bool>;

class Token
{
public:
    Token(TokenType type, ValueOfToken value = std::monostate()) : type(type), value(value) {}
    TokenType getType() const
    {
        return type;
    }
    ValueOfToken getValue() const
    {
        return value;
    }

protected:
    TokenType type;
    ValueOfToken value;
};

class TokenKeyword : public Token
{
public:
    TokenKeyword(TokenKeywordType type) : Token(type) {}
};

class TokenIntValue : public Token
{
public:
    TokenIntValue(TokenIntValueType type, int value) : Token(type, value) {};
};

class TokenFloatValue : public Token
{
public:
    TokenFloatValue(TokenFloatValueType type, float value) : Token(type, value) {};
};

class TokenBoolValue : public Token
{
public:
    TokenBoolValue(TokenBoolValueType type, bool value) : Token(type, value) {};
};

class TokenTextValue : public Token
{
public:
    TokenTextValue(TokenTextValueType type, const std::string &value) : Token(type, value) {};
};

class TokenOperator : public Token
{
public:
    TokenOperator(TokenOperatorType type) : Token(type) {}
};
