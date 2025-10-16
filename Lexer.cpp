#include "Lexer.h"
#include <cctype>
#include <iostream>

// Initialize keyword set
const std::unordered_set<std::string> Lexer::keywords = {
    "int", "float", "if", "else", "while", "for", "return", "char", "string", "bool", "true", "false", "void"
};

// Constructor
Lexer::Lexer(const std::string& input)
    : src(input), pos(0), line(1), column(1) {}

bool Lexer::isKeyword(const std::string& word) const {
    return keywords.count(word) > 0;
}

// Skip whitespace
void Lexer::skipWhitespace() {
    while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) {
        if (src[pos] == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }
        ++pos;
    }
}

// Skip comments (// or /* ... */)
void Lexer::skipComment() {
    if (pos + 1 >= src.size()) return;

    if (src[pos] == '/' && src[pos + 1] == '/') {
        pos += 2;
        column += 2;
        while (pos < src.size() && src[pos] != '\n') {
            ++pos; ++column;
        }
    } else if (src[pos] == '/' && src[pos + 1] == '*') {
        pos += 2;
        column += 2;
        while (pos + 1 < src.size()) {
            if (src[pos] == '*' && src[pos + 1] == '/') {
                pos += 2;
                column += 2;
                return;
            }
            if (src[pos] == '\n') {
                ++line;
                column = 1;
            } else {
                ++column;
            }
            ++pos;
        }
    }
}

// Get next token
Token Lexer::nextToken() {
    while (true) {
        skipWhitespace();

        if (pos >= src.size())
            return { TokenType::EOF_TOKEN, "", line, column };

        // Handle comments
        if (src[pos] == '/' && pos + 1 < src.size() &&
            (src[pos + 1] == '/' || src[pos + 1] == '*')) {
            skipComment();
            continue;
        }

        break;
    }

    char current = src[pos];

    // Identifiers or Keywords
    if (std::isalpha(static_cast<unsigned char>(current)) || current == '_') {
        size_t start = pos;
        int startCol = column;
        while (pos < src.size() &&
               (std::isalnum(static_cast<unsigned char>(src[pos])) || src[pos] == '_')) {
            ++pos; ++column;
        }
        std::string word = src.substr(start, pos - start);
        TokenType t = isKeyword(word) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
        return { t, word, line, startCol };
    }

    // Numbers
    if (std::isdigit(static_cast<unsigned char>(current))) {
        size_t start = pos;
        int startCol = column;
        bool isFloat = false;

        while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) {
            ++pos; ++column;
        }

        if (pos < src.size() && src[pos] == '.') {
            isFloat = true;
            ++pos; ++column;

            // digits after decimal
            while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) {
                ++pos; ++column;
            }
        }

        std::string number = src.substr(start, pos - start);
        return { isFloat ? TokenType::FLOAT : TokenType::INTEGER, number, line, startCol };
    }

    // String literal
    if (current == '"') {
        size_t start = pos;
        int startCol = column;
        ++pos; ++column;
        std::string value;
        bool closed = false;

        while (pos < src.size()) {
            char c = src[pos];
            if (c == '"') { closed = true; ++pos; ++column; break; }
            if (c == '\\' && pos + 1 < src.size()) {
                value += src[pos];
                value += src[pos + 1];
                pos += 2; column += 2;
            } else {
                if (c == '\n') { ++line; column = 1; }
                else { ++column; }
                value += c;
                ++pos;
            }
        }

        if (!closed) {
            return { TokenType::UNKNOWN, src.substr(start, pos - start), line, startCol };
        }

        return { TokenType::STRING, value, line, startCol };
    }

    // Char literal
    if (current == '\'') {
        size_t start = pos;
        int startCol = column;
        ++pos; ++column;
        std::string value;
        if (pos >= src.size()) return { TokenType::UNKNOWN, "'", line, startCol };

        if (src[pos] == '\\' && pos + 1 < src.size()) {
            value += src[pos];
            value += src[pos + 1];
            pos += 2; column += 2;
        } else {
            value += src[pos];
            ++pos; ++column;
        }

        if (pos < src.size() && src[pos] == '\'') {
            ++pos; ++column;
            return { TokenType::CHAR, value, line, startCol };
        } else {
            return { TokenType::UNKNOWN, src.substr(start, pos - start), line, startCol };
        }
    }

    // Operators (support ++, --, +=, -=, *=, /=, etc.)
    if (std::string("+-*/%=&|<>!").find(current) != std::string::npos) {
        int startCol = column;
        std::string op(1, current);

        if (pos + 1 < src.size()) {
            char next = src[pos + 1];
            std::string two = op + next;
            if (two == "++" || two == "--" || two == "==" || two == "!=" ||
                two == "<=" || two == ">=" || two == "+=" || two == "-=" ||
                two == "*=" || two == "/=" || two == "%=" ||
                two == "&&" || two == "||") {
                pos += 2; column += 2;
                return { TokenType::OPERATOR, two, line, startCol };
            }
        }

        ++pos; ++column;
        return { TokenType::OPERATOR, op, line, startCol };
    }

    // Separators
    if (std::string("(),;{}[]").find(current) != std::string::npos) {
        std::string sep(1, current);
        int startCol = column;
        ++pos; ++column;
        return { TokenType::SEPARATOR, sep, line, startCol };
    }

    // Unknown character
    std::string single(1, current);
    int startCol = column;
    ++pos; ++column;
    return { TokenType::UNKNOWN, single, line, startCol };
}

