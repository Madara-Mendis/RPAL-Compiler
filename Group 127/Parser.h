#ifndef RPAL_PARSER_H
#define RPAL_PARSER_H

#include <vector>
#include "Tokens\Token.h"
#include "Lexer.h"
#include "Tokens\TokenController.h"
#include "Tree\Tree.h"
#include "Tree\TreeNode.h"
using namespace std;

// function prototypes
void E();
void Ew();
void T();
void Ta();
void Tc();
void B();
void Bt();
void Bs();
void Bp();
void A();
void At();
void Af();
void Ap();
void R();
void Rn();
void D();
void Da();
void Dr();
void Db();
void Vb();
void Vl();

// parses input tokens and constructs ast
class Parser
{
public:
    static vector<TreeNode *> nodeStack;

    static void parse()
    {
        TokenController &tokenController = TokenController::getInstance();
        Token token = tokenController.top();

        // if input token equals end of file token
        if (token.type == tokenType::END_OF_FILE)
        {
            return;
        }
        else
        {
            E(); // parse expression

            // if next token is end of file token
            if (tokenController.top().type == tokenType::END_OF_FILE)
            {
                // set root of ast to last node in nodeStack
                Tree::getInstance().setASTRoot(Parser::nodeStack.back());
                return;
            }
            else
            {
                throw runtime_error("Syntax Error: end of file expected");
            }
        }
    }
};

vector<TreeNode *> Parser::nodeStack;

// builds tree with given parameters
void build_tree(const string &label, const int &num, const bool isLeaf, const string &value = "")
{
    TreeNode *node;

    // create either a leaf node or an internal node
    if (isLeaf)
    {
        node = new LeafNode(label, value);
    }
    else
    {
        node = new InternalNode(label);
    }

    // add children to newly created node
    for (int i = 0; i < num; i++)
    {
        node->addChild(Parser::nodeStack.back());
        Parser::nodeStack.pop_back();
    }

    // reverse children
    node->reverseChildren();

    Parser::nodeStack.push_back(node);
}

// parse E
void E()
{
    TokenController &tokenController = TokenController::getInstance();

    // if current token is "let"
    if (tokenController.top().value == "let")
    {
        tokenController.pop();
        D();

        // if next token is "in"
        if (tokenController.top().value == "in")
        {
            tokenController.pop();
            E();
        }
        else
        {
            throw runtime_error("Syntax Error: 'in' expected");
        }

        // make "let" node with 2 children
        build_tree("let", 2, false);
    }
    // if current token is "fn"
    else if (tokenController.top().value == "fn")
    {
        tokenController.pop();
        int n = 0;

        // process identifiers until a non-identifier token
        while (tokenController.top().type == tokenType::IDENTIFIER)
        {
            Vb();
            n++;
        }

        if (n == 0)
        {
            throw runtime_error("Syntax Error: at least one identifier expected");
        }

        // if next token is "."
        if (tokenController.top().value == ".")
        {
            tokenController.pop();
            E();
        }
        else
        {
            throw runtime_error("Syntax Error: '.' expected");
        }

        // make "lambda" node with n+1 children
        build_tree("lambda", n + 1, false);
    }
    else
    {
        Ew();
    }
}

// parse Ew
void Ew()
{
    TokenController &tokenController = TokenController::getInstance();
    T();

    // if next token is "where"
    if (tokenController.top().value == "where")
    {
        tokenController.pop();
        Dr();
        build_tree("where", 2, false);
    }
}

// parse T
void T()
{
    TokenController &tokenController = TokenController::getInstance();
    Ta();
    int n = 0;

    // process additional T expressions separated by commas
    while (tokenController.top().value == ",")
    {
        tokenController.pop();
        Ta();
        n++;
    }

    if (n > 0)
    {
        build_tree("tau", n + 1, false);
    }
}

// parse Ta
void Ta()
{
    TokenController &tokenController = TokenController::getInstance();
    Tc();

    // process additional Tc expressions separated by "aug"
    while (tokenController.top().value == "aug")
    {
        tokenController.pop();
        Tc();
        build_tree("aug", 2, false);
    }
}

// parse Tc
void Tc()
{
    TokenController &tokenController = TokenController::getInstance();
    B();

    // if next token is "->"
    if (tokenController.top().value == "->")
    {
        tokenController.pop();
        Tc();

        // if next token is "|"
        if (tokenController.top().value == "|")
        {
            tokenController.pop();
            Tc();
            build_tree("->", 3, false);
        }
        else
        {
            throw runtime_error("Syntax Error: '|' expected");
        }
    }
}

// parse B
void B()
{
    TokenController &tokenController = TokenController::getInstance();
    Bt();

    // process additional Bt expressions separated by "or"
    while (tokenController.top().value == "or")
    {
        tokenController.pop();
        Bt();
        build_tree("or", 2, false);
    }
}

// parse Bt
void Bt()
{
    TokenController &tokenController = TokenController::getInstance();
    Bs();

    // process additional Bs expressions separated by "&"
    while (tokenController.top().value == "&")
    {
        tokenController.pop();
        Bs();
        build_tree("&", 2, false);
    }
}

// parse Bs
void Bs()
{
    TokenController &tokenController = TokenController::getInstance();
    if (tokenController.top().value == "not")
    {
        tokenController.pop();
        Bp();
        build_tree("not", 1, false);
    }
    else
    {
        Bp();
    }
}

// parse Bp
void Bp()
{
    TokenController &tokenController = TokenController::getInstance();
    A();

    // if comparison operator
    if (tokenController.top().value == "gr" || tokenController.top().value == ">")
    {
        tokenController.pop();
        A();
        build_tree("gr", 2, false);
    }
    else if (tokenController.top().value == "ge" || tokenController.top().value == ">=")
    {
        tokenController.pop();
        A();
        build_tree("ge", 2, false);
    }
    else if (tokenController.top().value == "ls" || tokenController.top().value == "<")
    {
        tokenController.pop();
        A();
        build_tree("ls", 2, false);
    }
    else if (tokenController.top().value == "le" || tokenController.top().value == "<=")
    {
        tokenController.pop();
        A();
        build_tree("le", 2, false);
    }
    else if (tokenController.top().value == "eq" || tokenController.top().value == "=")
    {
        tokenController.pop();
        A();
        build_tree("eq", 2, false);
    }
    else if (tokenController.top().value == "ne" || tokenController.top().value == "!=")
    {
        tokenController.pop();
        A();
        build_tree("ne", 2, false);
    }
}

// parse A
void A()
{
    TokenController &tokenController = TokenController::getInstance();

    // if unary plus operator
    if (tokenController.top().value == "+")
    {
        tokenController.pop();
        At();
    }
    // if unary minus operator
    else if (tokenController.top().value == "-")
    {
        tokenController.pop();
        At();
        build_tree("neg", 1, false);
    }
    else
    {
        At();
    }

    // if addition and subtraction operators
    while (tokenController.top().value == "+" || tokenController.top().value == "-")
    {
        if (tokenController.top().value == "+")
        {
            tokenController.pop();
            At();
            build_tree("+", 2, false);
        }
        else if (tokenController.top().value == "-")
        {
            tokenController.pop();
            At();
            build_tree("-", 2, false);
        }
    }
}
/* ****************************************************************************************************************** */
// parse At
void At()
{
    TokenController &tokenController = TokenController::getInstance();
    Af();

    // if multiplication and division operators
    while (tokenController.top().value == "*" || tokenController.top().value == "/")
    {
        if (tokenController.top().value == "*")
        {
            tokenController.pop();
            Af();
            build_tree("*", 2, false);
        }
        else if (tokenController.top().value == "/")
        {
            tokenController.pop();
            Af();
            build_tree("/", 2, false);
        }
    }
}

// parse Af
void Af()
{
    TokenController &tokenController = TokenController::getInstance();
    Ap();

    // if exponentiation operator
    while (tokenController.top().value == "**")
    {
        tokenController.pop();
        Ap();
        build_tree("**", 2, false);
    }
}

// parse Ap
void Ap()
{
    TokenController &tokenController = TokenController::getInstance();
    R();

    // if function application operator
    while (tokenController.top().value == "@")
    {
        tokenController.pop();

        // if identifier token
        if (tokenController.top().type == tokenType::IDENTIFIER)
        {
            Token token = tokenController.pop();
            build_tree("identifier", 0, true, token.value);
        }
        else
        {
            throw runtime_error("Syntax Error: Identifier expected");
        }

        R();
        build_tree("@", 3, false);
    }
}

// parse R
void R()
{
    TokenController &tokenController = TokenController::getInstance();
    Rn();

    Token top = tokenController.top();
    while (top.type == tokenType::IDENTIFIER || top.type == tokenType::INTEGER || top.type == tokenType::STRING || top.value == "true" || top.value == "false" || top.value == "nil" || top.value == "(" || top.value == "dummy")
    {
        Rn();
        top = tokenController.top();
        build_tree("gamma", 2, false);
    }
}

// parse Rn
void Rn()
{
    TokenController &tokenController = TokenController::getInstance();
    Token top = tokenController.top();

    if (top.type == tokenType::IDENTIFIER)
    {
        // parse identifier
        Token token = tokenController.pop();
        build_tree("identifier", 0, true, token.value);
    }
    else if (top.type == tokenType::INTEGER)
    {
        // parse integer
        Token token = tokenController.pop();
        build_tree("integer", 0, true, token.value);
    }
    else if (top.type == tokenType::STRING)
    {
        // parse string
        Token token = tokenController.pop();
        build_tree("string", 0, true, token.value);
    }
    else if (top.value == "true")
    {
        // parse true
        tokenController.pop();
        build_tree("true", 0, true);
    }
    else if (top.value == "false")
    {
        // parse false
        tokenController.pop();
        build_tree("false", 0, true);
    }
    else if (top.value == "nil")
    {
        // parse nil
        tokenController.pop();
        build_tree("nil", 0, true);
    }
    else if (top.value == "(")
    {
        tokenController.pop();
        E();
        if (tokenController.top().value == ")")
        {
            tokenController.pop();
        }
        else
        {
            throw runtime_error("Syntax Error: ')' expected");
        }
    }
    else if (top.value == "dummy")
    {
        // parse dummy
        tokenController.pop();
        build_tree("dummy", 0, true);
    }
    else
    {
        throw runtime_error("Syntax Error: Identifier, Integer, String, 'true', 'false', 'nil', '(', 'dummy' expected\ngot: " + top.value);
    }
}

// parse D
void D()
{
    TokenController &tokenController = TokenController::getInstance();
    Da();

    while (tokenController.top().value == "within")
    {
        tokenController.pop();
        D();
        build_tree("within", 2, false);
    }
}

// parse Da
void Da()
{
    TokenController &tokenController = TokenController::getInstance();
    Dr();
    int n = 0;

    while (tokenController.top().value == "and")
    {
        tokenController.pop();
        Dr();
        n++;
    }
    if (n > 0)
    {
        build_tree("and", n + 1, false);
    }
}

// parse Dr
void Dr()
{
    TokenController &tokenController = TokenController::getInstance();

    if (tokenController.top().value == "rec")
    {
        tokenController.pop();
        Db();
        build_tree("rec", 1, false);
    }
    else
    {
        Db();
    }
}

// parse Db
void Db()
{
    TokenController &tokenController = TokenController::getInstance();

    if (tokenController.top().value == "(")
    {
        tokenController.pop();
        D();

        if (tokenController.top().value == ")")
        {
            tokenController.pop();
        }
        else
        {
            throw runtime_error("Syntax Error: ')' expected");
        }
    }
    else if (tokenController.top().type == tokenType::IDENTIFIER)
    {
        // parse identifier
        Token token = tokenController.pop();
        build_tree("identifier", 0, true, token.value);

        if (tokenController.top().value == ",")
        {
            tokenController.pop();
            Vl();

            if (tokenController.top().value == "=")
            {
                tokenController.pop();
                E();
                build_tree("=", 2, false);
            }
            else
            {
                throw runtime_error("Syntax Error: '=' expected");
            }
        }
        else
        {
            int n = 0;

            while (tokenController.top().value != "=" && tokenController.top().type == tokenType::IDENTIFIER)
            {
                Vb();
                n++;
            }

            if (tokenController.top().value == "(")
            {
                Vb();
                n++;
            }

            if (n == 0 && tokenController.top().value == "=")
            {
                tokenController.pop();
                E();
                build_tree("=", 2, false);
            }
            else if (n != 0 && tokenController.top().value == "=")
            {
                tokenController.pop();
                E();
                build_tree("fcn_form", n + 2, false);
            }
            else
            {
                throw runtime_error("Syntax Error: '=' expected");
            }
        }
    }
    else
    {
        throw runtime_error("Syntax Error: '(' or Identifier expected");
    }
}

// parse Vb
void Vb()
{
    TokenController &tokenController = TokenController::getInstance();

    if (tokenController.top().type == tokenType::IDENTIFIER)
    {
        // parse identifier
        Token token = tokenController.pop();
        build_tree("identifier", 0, true, token.value);
    }
    else if (tokenController.top().value == "(")
    {
        tokenController.pop();

        if (tokenController.top().value == ")")
        {
            tokenController.pop();
            build_tree("()", 0, true);
        }
        else if (tokenController.top().type == tokenType::IDENTIFIER)
        {
            // parse identifier
            Token token = tokenController.pop();
            build_tree("identifier", 0, true, token.value);

            if (tokenController.top().value == ",")
            {
                tokenController.pop();
                Vl();
            }

            if (tokenController.top().value == ")")
            {
                tokenController.pop();
            }
            else
            {
                throw runtime_error("Syntax Error: ')' expected");
            }
        }
        else
        {
            throw runtime_error("Syntax Error: Identifier or ')' expected");
        }
    }
    else
    {
        throw runtime_error("Syntax Error: Identifier or '(' expected");
    }
}

// parse Vl
void Vl()
{
    TokenController &tokenController = TokenController::getInstance();

    if (tokenController.top().type == tokenType::IDENTIFIER)
    {
        // parse identifier
        Token token = tokenController.pop();
        build_tree("identifier", 0, true, token.value);

        int n = 2;
        while (tokenController.top().value == ",")
        {
            tokenController.pop();
            token = tokenController.pop();
            build_tree("identifier", 0, true, token.value);
            n++;
        }

        build_tree(",", n, false);
    }
    else
    {
        throw runtime_error("Syntax Error: Identifier expected");
    }
}

#endif // RPAL_PARSER_H