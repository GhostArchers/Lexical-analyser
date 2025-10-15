// Lexer.h
#pragma once

#include <string>
#include <unordered_set>
#include "Utility.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);

    // Get next token from the source
    Token nextToken();

private:
    std::string src;
    std::size_t index;
    int line;
    int column;

    static const std::unordered_set<std::string> KEYWORDS;

    char peek(int lookahead = 0) const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();

    static bool isAlpha(char c);
    static bool isDigit(char c);
    static bool isAlphaNumericOrUnderscore(char c);

    Token makeIdentifierOrKeyword();
    Token makeNumber();
    Token makeString();
    Token makeChar();
    Token makeOperatorOrSeparator();
};


