#pragma once
#include <string>
#include <vector>
#include <unordered_set>

// Token types
enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    INTEGER,
    FLOAT,
    STRING,
    CHAR,
    OPERATOR,
    SEPARATOR,
    COMMENT,
    UNKNOWN,
    EOF_TOKEN
};

// Token structure
struct Token {
    TokenType tkType;
    std::string value;
    int line;
    int column;
};

class Lexer {
public:
    explicit Lexer(const std::string& input);
    Token nextToken();

private:
    std::string src;
    size_t pos;
    int line, column;

    static const std::unordered_set<std::string> keywords;

    bool isKeyword(const std::string& word) const;
    void skipWhitespace();
    void skipComment();
};
