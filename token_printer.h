#include <iostream>
#include "token.h"

inline std::string to_string(TokenKeywordType t)
{
    switch (t)
    {
    case TokenKeywordType::K_INT_TYPE:
        return "K_INT_TYPE";
    case TokenKeywordType::K_FLOAT_TYPE:
        return "K_FLOAT_TYPE";
    case TokenKeywordType::K_BOOL_TYPE:
        return "K_BOOL_TYPE";
    case TokenKeywordType::K_TEXT_TYPE:
        return "K_TEXT_TYPE";
    case TokenKeywordType::K_NUMBER_TYPE:
        return "K_NUMBER_TYPE";
    case TokenKeywordType::K_STRUCT_TYPE:
        return "K_STRUCT_TYPE";
    case TokenKeywordType::K_LIST_TYPE:
        return "K_LIST_TYPE";
    case TokenKeywordType::K_ANY_TYPE:
        return "K_ANY_TYPE";
    case TokenKeywordType::K_NULL:
        return "K_NULL";
    case TokenKeywordType::K_REF:
        return "K_REF";
    case TokenKeywordType::K_CONST:
        return "K_CONST";
    case TokenKeywordType::K_IF:
        return "K_IF";
    case TokenKeywordType::K_WHILE:
        return "K_WHILE";
    case TokenKeywordType::K_REPEAT:
        return "K_REPEAT";
    case TokenKeywordType::K_FOR:
        return "K_FOR";
    case TokenKeywordType::K_AND:
        return "K_AND";
    case TokenKeywordType::K_OR:
        return "K_OR";
    case TokenKeywordType::K_FUNCTION:
        return "K_FUNCTION";
    case TokenKeywordType::K_RETURN:
        return "K_RETURN";
    case TokenKeywordType::K_DECORATE:
        return "K_DECORATE";
    case TokenKeywordType::K_BIND:
        return "K_BIND";
    case TokenKeywordType::K_INCLUDE:
        return "K_INCLUDE";
    default:
        return "UNKNOWN_KEYWORD";
    }
}

inline std::string to_string(TokenIntValueType t)
{
    return "V_INT";
}

inline std::string to_string(TokenBoolValueType t)
{
    return "V_BOOL";
}

inline std::string to_string(TokenFloatValueType t)
{
    return "V_FLOAT";
}

inline std::string to_string(TokenTextValueType t)
{
    switch (t)
    {
    case TokenTextValueType::V_TEXT:
        return "V_TEXT";
    case TokenTextValueType::V_IDENTIFIER:
        return "V_IDENTIFIER";
    case TokenTextValueType::V_COMMENT:
        return "V_COMMENT";
    default:
        return "UNKNOWN_TEXT";
    }
}

inline std::string to_string(TokenOperatorType t)
{
    switch (t)
    {
    case TokenOperatorType::O_LEFT_PAREN:
        return "O_LEFT_PAREN";
    case TokenOperatorType::O_RIGHT_PAREN:
        return "O_RIGHT_PAREN";
    case TokenOperatorType::O_LEFT_BRACE:
        return "O_LEFT_BRACE";
    case TokenOperatorType::O_RIGHT_BRACE:
        return "O_RIGHT_BRACE";
    case TokenOperatorType::O_SEMICOLON:
        return "O_SEMICOLON";
    case TokenOperatorType::O_COMMA:
        return "O_COMMA";
    case TokenOperatorType::O_DOT:
        return "O_DOT";
    case TokenOperatorType::O_PLUS:
        return "O_PLUS";
    case TokenOperatorType::O_MINUS:
        return "O_MINUS";
    case TokenOperatorType::O_ASTERISK:
        return "O_ASTERISK";
    case TokenOperatorType::O_SLASH:
        return "O_SLASH";
    case TokenOperatorType::O_BACKSLASH:
        return "O_BACKSLASH";
    case TokenOperatorType::O_ASSIGN:
        return "O_ASSIGN";
    case TokenOperatorType::O_EQUAL:
        return "O_EQUAL";
    case TokenOperatorType::O_LESS:
        return "O_LESS";
    case TokenOperatorType::O_GREATER:
        return "O_GREATER";
    case TokenOperatorType::O_LESS_EQUAL:
        return "O_LESS_EQUAL";
    case TokenOperatorType::O_GREATER_EQUAL:
        return "O_GREATER_EQUAL";
    case TokenOperatorType::O_ARROW:
        return "O_ARROW";
    case TokenOperatorType::O_MODULO:
        return "O_MODULO";
    case TokenOperatorType::O_DIV:
        return "O_DIV";
    default:
        return "UNKNOWN_OPERATOR";
    }
}

inline std::string value_to_string(const ValueOfToken &value)
{
    return std::visit([](auto &&arg) -> std::string
                      {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>)
            return "";
        else if constexpr (std::is_same_v<T, std::string>)
            return "\"" + arg + "\"";
        else if constexpr (std::is_same_v<T, int>)
            return std::to_string(arg);
        else if constexpr (std::is_same_v<T, float>)
            return std::to_string(arg);
        else if constexpr (std::is_same_v<T, bool>)
            return arg ? "true" : "false";
        else
            return "UNKNOWN_VALUE"; }, value);
}

inline std::string token_type_to_string(const TokenType &type)
{
    return std::visit([](auto &&arg) -> std::string
                      { return to_string(arg); }, type);
}

inline std::ostream &operator<<(std::ostream &os, const Token &token)
{
    os << token_type_to_string(token.getType());
    std::string val = value_to_string(token.getValue());
    if (!val.empty())
    {
        os << " : " << val;
    }
    return os;
}
