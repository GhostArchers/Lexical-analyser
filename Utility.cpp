#include "Utility.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::SEPARATOR: return "SEPARATOR";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::EOF_TOKEN: return "EOF";
    }
    return "UNKNOWN";
}

