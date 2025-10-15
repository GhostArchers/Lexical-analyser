// Lexer.cpp
#include "Lexer.h"

#include <cctype>

const std::unordered_set<std::string> Lexer::KEYWORDS = {
    "int", "float", "char", "if", "else", "for", "while", "do", "break", "string"
};

Lexer::Lexer(const std::string& source)
    : src(source), index(0), line(1), column(1) {}

char Lexer::peek(int lookahead) const {
    std::size_t pos = index + static_cast<std::size_t>(lookahead);
    if (pos >= src.size()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    char c = src[index++];
    if (c == '\n') {
        line += 1;
        column = 1;
    } else {
        column += 1;
    }
    return c;
}

bool Lexer::isAtEnd() const {
    return index >= src.size();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

bool Lexer::isAlpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Lexer::isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool Lexer::isAlphaNumericOrUnderscore(char c) {
    return isAlpha(c) || isDigit(c) || c == '_';
}

Token Lexer::makeIdentifierOrKeyword() {
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;
    while (!isAtEnd() && isAlphaNumericOrUnderscore(peek())) {
        value.push_back(advance());
    }

    TokenType type = KEYWORDS.count(value) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
    return Token{type, value, tokenLine, tokenColumn};
}

Token Lexer::makeNumber() {
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;
    bool seenDot = false;

    while (!isAtEnd()) {
        char c = peek();
        if (isDigit(c)) {
            value.push_back(advance());
        } else if (c == '.' && !seenDot && isDigit(peek(1))) {
            seenDot = true;
            value.push_back(advance());
        } else {
            break;
        }
    }

    TokenType type = seenDot ? TokenType::FLOAT : TokenType::INTEGER;
    return Token{type, value, tokenLine, tokenColumn};
}

Token Lexer::makeString() {
    // Starting '"' already consumed by caller
    int tokenLine = line;
    int tokenColumn = column; // This column points to position after the opening quote

    std::string value;
    bool terminated = false;
    while (!isAtEnd()) {
        char c = advance();
        if (c == '"') {
            terminated = true;
            break;
        }
        if (c == '\\') {
            // Escape sequence: include next char as-is if present
            if (!isAtEnd()) {
                char next = advance();
                value.push_back('\\');
                value.push_back(next);
                continue;
            }
        }
        value.push_back(c);
    }

    if (!terminated) {
        return Token{TokenType::UNKNOWN, value, tokenLine, tokenColumn - 1};
    }
    return Token{TokenType::STRING, value, tokenLine, tokenColumn - 1};
}

Token Lexer::makeChar() {
    // Starting '\'' already consumed by caller
    int tokenLine = line;
    int tokenColumn = column; // position after opening quote

    std::string value;
    if (isAtEnd()) {
        return Token{TokenType::UNKNOWN, value, tokenLine, tokenColumn - 1};
    }

    char c = advance();
    if (c == '\\') {
        // escape sequence: include escaped char
        if (!isAtEnd()) {
            char next = advance();
            value.push_back('\\');
            value.push_back(next);
        } else {
            return Token{TokenType::UNKNOWN, value, tokenLine, tokenColumn - 1};
        }
    } else {
        value.push_back(c);
    }

    // expect closing quote
    if (peek() == '\'') {
        advance();
        return Token{TokenType::CHAR, value, tokenLine, tokenColumn - 1};
    }

    return Token{TokenType::UNKNOWN, value, tokenLine, tokenColumn - 1};
}

Token Lexer::makeOperatorOrSeparator() {
    int tokenLine = line;
    int tokenColumn = column;

    char c = advance();
    std::string value(1, c);

    // Multi-char operators: ==, !=, <=, >=
    if ((c == '=' || c == '!' || c == '<' || c == '>') && peek() == '=') {
        value.push_back(advance());
        return Token{TokenType::OPERATOR, value, tokenLine, tokenColumn};
    }

    // Single-char operators
    switch (c) {
        case '+': case '-': case '*': case '/': case '%':
        case '=': case '<': case '>':
            return Token{TokenType::OPERATOR, value, tokenLine, tokenColumn};
        case ';': case ',': case '(': case ')': case '{': case '}': case '[': case ']':
            return Token{TokenType::SEPARATOR, value, tokenLine, tokenColumn};
        default:
            return Token{TokenType::UNKNOWN, value, tokenLine, tokenColumn};
    }
}

Token Lexer::nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return Token{TokenType::EOF_TOKEN, "", line, column};
    }

    char c = peek();

    if (isAlpha(c)) {
        return makeIdentifierOrKeyword();
    }

    if (isDigit(c)) {
        return makeNumber();
    }

    if (c == '"') {
        advance(); // consume opening quote
        return makeString();
    }

    if (c == '\'') {
        advance(); // consume opening quote
        return makeChar();
    }

    return makeOperatorOrSeparator();
}


