#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

// Token types enumeration.
enum tokenType
{
    IDENTIFIER,
    INTEGER,
    STRING,
    OPERATOR,
    DELIMITER,
    KEYWORD,
    END_OF_FILE
};

// Token structure.
struct Token
{
    tokenType type;
    string value;
};

#endif // TOKEN_H