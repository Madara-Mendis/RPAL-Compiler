#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
#include "1scanner.h"

int main() {
    std::unordered_set<std::string> keywords = {"let", "where", "within", "aug", "fn", "in", "rec"};
    std::unordered_set<std::string> operators = {"and", "or", "not", "gr", "ge", "ls", "le", "eq", "ne"};
    std::unordered_set<std::string> booleanValues = {"true", "false"};

    std::ifstream file("RPAl.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file RPAl.txt" << std::endl;
        return 1;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::vector<Token> tokens = lex(content, keywords, operators, booleanValues);
    for (const auto& token : tokens) {
        std::cout << tokenTypeToString(token.type) << " " << token.value << std::endl;
    }

    return 0;
}