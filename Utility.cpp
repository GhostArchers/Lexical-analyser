// Utility.cpp
#include "Utility.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::SEPARATOR: return "SEPARATOR";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}


