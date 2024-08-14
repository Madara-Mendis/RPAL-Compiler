#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

enum class TokenType {
    KEYWORD, OPERATOR, BOOLEAN, IDENTIFIER, INTEGER, STRING, PUNCTUATION, COMMENT, WHITESPACE, END_OF_LINE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD: return "<KEYWORD>";
        case TokenType::OPERATOR: return "<OPERATOR>";
        case TokenType::BOOLEAN: return "<BOOLEAN>";
        case TokenType::IDENTIFIER: return "<IDENTIFIER>";
        case TokenType::INTEGER: return "<INTEGER>";
        case TokenType::STRING: return "<STRING>";
        case TokenType::PUNCTUATION: return "<PUNCTUATION>";
        case TokenType::COMMENT: return "<COMMENT>";
        case TokenType::WHITESPACE: return "<WHITESPACE>";
        case TokenType::END_OF_LINE: return "<END_OF_LINE>";
        default: return "<UNKNOWN>";
    }
}

bool isOperator(char ch) {
    std::string operatorSymbols = "+-*/<>&.@/:=~|$!#%^_[]{}\"'?,";
    return operatorSymbols.find(ch) != std::string::npos;
}

bool isPunctuation(char ch) {
    std::string punctuation = "();,";
    return punctuation.find(ch) != std::string::npos;
}

std::vector<Token> lex(const std::string& input,
                       const std::unordered_set<std::string>& keywords,
                       const std::unordered_set<std::string>& operators,
                       const std::unordered_set<std::string>& booleanValues) {
    std::vector<Token> tokens;
    std::string token;
    for (size_t i = 0; i < input.length(); ++i) {
        if (std::isspace(input[i])) {
            if (input[i] == '\n') {
                tokens.push_back({TokenType::END_OF_LINE, "\\n"});
            } else {
                tokens.push_back({TokenType::WHITESPACE, " "});
            }
        } else if (std::isalpha(input[i]) || input[i] == '_') {
            token += input[i];
            while (i + 1 < input.length() && (std::isalnum(input[i + 1]) || input[i + 1] == '_')) {
                token += input[++i];
            }
            if (keywords.find(token) != keywords.end()) {
                tokens.push_back({TokenType::KEYWORD, token});
            } else if (operators.find(token) != operators.end()) {
                tokens.push_back({TokenType::OPERATOR, token});
            } else if (booleanValues.find(token) != booleanValues.end()) {
                tokens.push_back({TokenType::BOOLEAN, token});
            } else {
                tokens.push_back({TokenType::IDENTIFIER, token});
            }
            token.clear();
        } else if (input[i] == '/' && i + 1 < input.length() && input[i + 1] == '/') {
            // Comment detected
            token += input[i++];
            token += input[i];
            while (i + 1 < input.length() && input[i + 1] != '\n') {
                token += input[++i];
            }
            tokens.push_back({TokenType::COMMENT, token});
        } else if (input[i] == '"') {
            // String detected
            token += input[i];
            while (i + 1 < input.length() && input[i + 1] != '"') {
                token += input[++i];
            }
            if (i + 1 < input.length()) {
                token += input[++i]; // Include closing quote
            }
            tokens.push_back({TokenType::STRING, token});
        } else if (std::isdigit(input[i])) {
            // Integer detected
            token += input[i];
            while (i + 1 < input.length() && std::isdigit(input[i + 1])) {
                token += input[++i];
            }
            tokens.push_back({TokenType::INTEGER, token});
        } else if (isOperator(input[i])) {
            // Operator detected
            token += input[i];
            while (i + 1 < input.length() and isOperator(input[i + 1])) {
                token += input[++i];
            }
            tokens.push_back({TokenType::OPERATOR, token});
        } else if (isPunctuation(input[i])) {
            // Punctuation detected
            token += input[i];
            tokens.push_back({TokenType::PUNCTUATION, token});
        } else {
            // Unknown character detected
            token += input[i];
            tokens.push_back({TokenType::UNKNOWN, token});
        }
        token.clear();
    }
    return tokens;
}


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
