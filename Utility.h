// Utility.h
#pragma once

#include <string>

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    STRING,
    CHAR,
    OPERATOR,
    SEPARATOR,
    EOF_TOKEN,
    UNKNOWN
};

struct Token {
    TokenType tkType;
    std::string value;
    int line;
    int column;
};

// Convert TokenType to human-readable string
std::string tokenTypeToString(TokenType type);


