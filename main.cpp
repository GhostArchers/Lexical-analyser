#include "Lexer.h"
#include <iostream>
#include <iomanip>

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::KEYWORD:    return "KEYWORD";
        case TokenType::INTEGER:    return "INTEGER";
        case TokenType::FLOAT:      return "FLOAT";
        case TokenType::STRING:     return "STRING";
        case TokenType::CHAR:       return "CHAR";
        case TokenType::OPERATOR:   return "OPERATOR";
        case TokenType::SEPARATOR:  return "SEPARATOR";
        case TokenType::COMMENT:    return "COMMENT";
        case TokenType::UNKNOWN:    return "UNKNOWN";
        case TokenType::EOF_TOKEN:  return "EOF";
        default:                    return "UNKNOWN";
    }
}

int main() {
    std::string input = R"(
        int main() {
            float num = 3.14;
            char c = 'a';
            string s = "Hello World";
            // This is a comment
            if (num > 1.0) {
                num += 2;
            }
        }
    )";

    std::cout << "=== LEXICAL ANALYSIS START ===\n";
    Lexer lexer(input);
    
    Token token;
    int count = 1;

    // Print table header
    std::cout << std::left << std::setw(6) << "No."
              << std::setw(15) << "Type"
              << std::setw(20) << "Value"
              << std::setw(8) << "Line"
              << "Column\n";
    std::cout << "--------------------------------------------------------------\n";

    // Generate tokens
    while (true) {
        token = lexer.nextToken();
        if (token.tkType == TokenType::EOF_TOKEN)
            break;

        std::cout << std::left << std::setw(6) << count++
                  << std::setw(15) << tokenTypeToString(token.tkType)
                  << std::setw(20) << token.value
                  << std::setw(8) << token.line
                  << token.column << "\n";
    }

    std::cout << "=== LEXICAL ANALYSIS COMPLETE ===\n";
    return 0;
}
