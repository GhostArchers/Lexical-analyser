// main.cpp
#include <iostream>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Utility.h"

int main() {
    std::cout << "=== Lexical Analyzer (C++) ===" << std::endl;

    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) {
            break;
        }
        if (input == "exit") {
            break;
        }

        Lexer lexer(input);
        std::cout << "--- Tokens ---" << std::endl;
        while (true) {
            Token token = lexer.nextToken();
            if (token.tkType == TokenType::EOF_TOKEN) {
                break;
            }
            std::cout
                << "Type: " << tokenTypeToString(token.tkType)
                << ", Value: '" << token.value << "'"
                << ", Line: " << token.line
                << ", Column: " << token.column
                << std::endl;
        }
    }

    std::cout << "[Program Terminated]" << std::endl;
    return 0;
}


