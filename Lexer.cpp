// Lexer.cpp
#include "Lexer.h"
#include <cctype>
#include <iostream>

// Constructor
Lexer::Lexer(const std::string& input)
    : src(input), pos(0), line(1), column(1) {}

// check if a word is a keyword
bool Lexer::isKeyword(const std::string& word) {
    for (const auto& kw : keywords)
        if (kw == word) return true;
    return false;
}

// skip whitespace and maintain line/column
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

Token Lexer::nextToken() {
    skipWhitespace();

    if (pos >= src.size())
        return { TokenType::EOF_TOKEN, "", line, column };

    char current = src[pos];

    // Identifiers and Keywords (letters and underscores, then letters/digits/_)
    if (std::isalpha(static_cast<unsigned char>(current)) || current == '_') {
        size_t start = pos;
        int startCol = column;
        while (pos < src.size() && (std::isalnum(static_cast<unsigned char>(src[pos])) || src[pos] == '_')) {
            ++pos;
            ++column;
        }
        std::string word = src.substr(start, pos - start);
        TokenType t = isKeyword(word) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
        return { t, word, line, startCol };
    }

    // Numbers: integer or float (simple)
    if (std::isdigit(static_cast<unsigned char>(current))) {
        size_t start = pos;
        int startCol = column;
        bool isFloat = false;
        while (pos < src.size() && (std::isdigit(static_cast<unsigned char>(src[pos])) || src[pos] == '.')) {
            if (src[pos] == '.') {
                if (isFloat) {
                    // second dot -> stop (we'll treat rest as unknown or separate tokens)
                    break;
                }
                isFloat = true;
            }
            ++pos;
            ++column;
        }
        std::string number = src.substr(start, pos - start);
        return { isFloat ? TokenType::FLOAT : TokenType::INTEGER, number, line, startCol };
    }

    // String literal "..."
    if (current == '"') {
        size_t start = pos;
        int startCol = column;
        ++pos; ++column; // skip opening quote
        std::string value;
        bool closed = false;
        while (pos < src.size()) {
            if (src[pos] == '"') {
                closed = true;
                ++pos; ++column; // consume closing quote
                break;
            }
            // simplistic escape handling: allow \" inside string
            if (src[pos] == '\\' && pos + 1 < src.size()) {
                value += src[pos];
                value += src[pos + 1];
                pos += 2;
                column += 2;
                continue;
            }
            value += src[pos];
            ++pos; ++column;
        }
        if (!closed) {
            // unterminated string — return as UNKNOWN token with what we collected
            return { TokenType::UNKNOWN, std::string(src.substr(start, pos - start)), line, startCol };
        }
        return { TokenType::STRING, value, line, startCol };
    }

    // Char literal 'a'
    if (current == '\'') {
        size_t start = pos;
        int startCol = column;
        ++pos; ++column; // skip opening quote
        if (pos >= src.size()) {
            // malformed
            return { TokenType::UNKNOWN, std::string(1, '\''), line, startCol };
        }
        std::string value;
        if (src[pos] == '\\' && pos + 1 < src.size()) {
            // escaped char like '\n' or '\''
            value += src[pos];
            value += src[pos + 1];
            pos += 2;
            column += 2;
        } else {
            value += src[pos];
            ++pos; ++column;
        }

        // expect closing quote
        if (pos < src.size() && src[pos] == '\'') {
            ++pos; ++column; // consume closing quote
            return { TokenType::CHAR, value, line, startCol };
        } else {
            // malformed char literal
            return { TokenType::UNKNOWN, std::string(src.substr(start, pos - start)), line, startCol };
        }
    }

    // Operators (single and two-char like ==, !=, <=, >=)
    if (std::string("+-*/%=&|<>!").find(current) != std::string::npos) {
        int startCol = column;
        std::string op(1, current);
        // check two-character operators
        if (pos + 1 < src.size()) {
            char next = src[pos + 1];
            if ((current == '=' && next == '=') ||
                (current == '!' && next == '=') ||
                (current == '<' && next == '=') ||
                (current == '>' && next == '=') ||
                (current == '&' && next == '&') ||
                (current == '|' && next == '|')) {
                op += next;
                pos += 2;
                column += 2;
                return { TokenType::OPERATOR, op, line, startCol };
            }
        }
        // single char operator
        ++pos;
        ++column;
        return { TokenType::OPERATOR, op, line, startCol };
    }

    // Separators
    if (std::string("(),;{}[]").find(current) != std::string::npos) {
        std::string sep(1, current);
        int startCol = column;
        ++pos;
        ++column;
        return { TokenType::SEPARATOR, sep, line, startCol };
    }

    // Anything else: unknown single char
    std::string single(1, current);
    int startCol = column;
    ++pos;
    ++column;
    return { TokenType::UNKNOWN, single, line, startCol };
}

