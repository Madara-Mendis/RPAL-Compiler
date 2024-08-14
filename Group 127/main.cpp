#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include "Parser.h"
#include "Tokens\Token.h"
#include "CSEdu.h"
using namespace std;

// map the enum values
unordered_map<tokenType, string> token_typeNames = {
    {tokenType::IDENTIFIER, "IDENTIFIER"},
    {tokenType::INTEGER, "INTEGER"},
    {tokenType::STRING, "STRING"},
    {tokenType::OPERATOR, "OPERATOR"},
    {tokenType::DELIMITER, "DELIMITER"},
    {tokenType::KEYWORD, "KEYWORD"},
    {tokenType::END_OF_FILE, "END_OF_FILE"}};

// get token name
string gettoken_typeName(tokenType type)
{
    if (token_typeNames.count(type) > 0)
    {
        return token_typeNames[type];
    }
    return "UNKNOWN";
}

// check if Graphviz is installed
bool isGraphvizInstalled()
{
// Redirect stderr to /dev/null or nul to discard error message
#ifdef _WIN32
    const char *redirectCommand = "2>nul";
#else
    const char *redirectCommand = "2>/dev/null";
#endif

    string command = "dot -V ";
    command += redirectCommand;

    int exitCode = system(command.c_str());
    return (exitCode == 0);
}

// display a warning message if Graphviz is not installed
void displayGraphvizWarning()
{
    cout << "Graphviz not installed. Download Graphviz from: https://graphviz.org/download/" << endl;
}

// helper function to generate dot file
int generateDotFileHelper(TreeNode *node, ofstream &file, int parent = -1, int nodeCount = 0)
{
    int currentNode = nodeCount;

    // increment nodeCount
    int nextNodeCount = nodeCount + 1;

    // set colors
    string labelColor = "darkblue";
    string valueColor = "darkgreen";
    string fillColor = (node->getValue() == " " || node->getValue().empty()) ? "#CCCCCC" : "#FFFFFF";

    // escape label characters if necessary
    string escapedLabel = node->getLabel();

    size_t pos1 = escapedLabel.find('&');
    while (pos1 != string::npos)
    {
        escapedLabel.replace(pos1, 1, "&amp;");
        pos1 = escapedLabel.find('&', pos1 + 5);
    }

    size_t pos = escapedLabel.find('>');
    while (pos != string::npos)
    {
        escapedLabel.replace(pos, 1, "&gt;");
        pos = escapedLabel.find('>', pos + 4);
    }

    // prepare label and value strings for the dot file
    string labelStr = (escapedLabel.empty()) ? "&nbsp;" : escapedLabel;
    string valueStr = (node->getValue().empty()) ? "&nbsp;" : node->getValue();

    size_t pos2 = valueStr.find('\n');

    while (pos2 != string::npos)
    {
        valueStr.replace(pos2, 1, "\\n");
        pos2 = valueStr.find('\n', pos2 + 2);
    }

    // write node details to the dot file
    file << "    node" << currentNode << " [label=<";
    file << "<font color=\"" << labelColor << "\">" << labelStr << "</font><br/>";
    file << "<font color=\"" << valueColor << "\">" << valueStr << "</font>";
    file << ">, style=filled, fillcolor=\"" << fillColor << "\"];\n";

    // create an edge between the current node and its parent
    if (currentNode != 0 && parent != -1)
    {
        file << "    node" << parent << " -> node" << currentNode << ";\n";
    }

    // recursively generate dot file contents for each child node
    for (TreeNode *child : node->getChildren())
    {
        nextNodeCount = generateDotFileHelper(child, file, currentNode, nextNodeCount);
    }

    return nextNodeCount;
}

// generate dot file for the AST
void generateDotFile(TreeNode *root, string filename)
{
    ;

    string f_name = filesystem::current_path().string() + filename;
    ofstream file(f_name);
    if (file.is_open())
    {
        file << "digraph Tree {\n";
        generateDotFileHelper(root, file, 0);
        file << "}\n";
        file.close();
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || string(argv[1]) == "-visualize")
    {
        cout << "ERROR: Usage: .\\rpal20 input_file [-visualize=VALUE]\n"
             << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Unable to open file: " << filename << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Check if the "-visualize" argument is provided
    string visualizeArg;
    bool visualizeAst = false;
    bool visualizeSt = false;

    for (int i = 2; i < argc; ++i)
    {
        string arg(argv[i]);

        if (arg == "-visualize")
        {
            visualizeAst = true;
            visualizeSt = true;
        }
        else if (arg == "-visualize=ast")
        {
            visualizeAst = true;
        }
        else if (arg == "-visualize=st")
        {
            visualizeSt = true;
        }
    }

    if (!isGraphvizInstalled() && (visualizeAst || visualizeSt))
    {
        displayGraphvizWarning();
        visualizeAst = false;
        visualizeSt = false;
    }

    if (isGraphvizInstalled() && (visualizeSt || visualizeAst))
    {
        if (!filesystem::exists("Visualizations"))
        {
            filesystem::create_directory("Visualizations");
        }
    }

    Lexer lexer(input);

    TokenController &tokenController = TokenController::getInstance();
    tokenController.setLexer(lexer);

    Parser::parse();
    TokenController::destroyInstance();

    TreeNode *root = Tree::getInstance().getASTRoot();

    if (visualizeAst)
    {
        // generate DOT file
        generateDotFile(root, "ast.dot");

        string dotFilePath = filesystem::current_path().string() + "\ast.dot";
        string outputFilePath = filesystem::current_path().string() + "\ast.png";

        string command = "dot -Tpng -Gdpi=150 " + dotFilePath + " -o " + outputFilePath;
        system(command.c_str());

        cout << "Find the ast.png in the current directory." << endl;
    }

    Tree::generate();
    TreeNode *st_root = Tree::getInstance().getSTRoot();

    if (visualizeSt)
    {
        generateDotFile(st_root, "st.dot");

        string dotFilePath_st = filesystem::current_path().string() + "\\st.dot";
        string outputFilePath_st = filesystem::current_path().string() + "\\st.png";

        string command_st = "dot -Tpng -Gdpi=150 " + dotFilePath_st + " -o " + outputFilePath_st;
        system(command_st.c_str());

        cout << "Find the ast.png in the current directory." << endl;
    }

    CSE cse = CSE();
    cse.createCS(Tree::getInstance().getSTRoot());
    cse.evaluate();

    cout << endl;

    return 0;
}