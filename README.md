# RPAL Compiler 

### **Overview**

This project is a fully functional compiler for the RPAL (Right-associative Positive Applicative Language) programming language. The compiler consists of three main components:

Scanner: Tokenizes the input source code, breaking it down into a series of tokens that represent the smallest meaningful elements of the code.

Parser: Analyzes the token sequence generated by the scanner, ensuring that the syntax conforms to the grammar of RPAL. It produces a syntactic structure (parse tree or abstract syntax tree).

CSE Machine: Executes the abstract syntax tree generated by the parser. The CSE machine evaluates the expressions according to the semantics of RPAL, producing the final output.

### **Components**

1. Scanner
Description: The scanner reads the source code and identifies lexical tokens such as identifiers, keywords, operators, literals, and delimiters.
Functionality: It performs lexical analysis and outputs a stream of tokens for the parser to process.
Implementation: The scanner uses regular expressions and finite state machines to recognize patterns in the source code.

2. Parser
Description: The parser takes the token stream from the scanner and applies the grammatical rules of RPAL to build a parse tree or abstract syntax tree.
Functionality: It ensures that the code adheres to the syntax rules of RPAL and organizes the tokens into a hierarchical structure that represents the logical structure of the program.
Implementation: The parser is implemented using recursive descent parsing, which handles the recursive nature of RPAL's grammar.

3. CSE Machine
Description: The Control Stack Execution (CSE) machine evaluates the abstract syntax tree generated by the parser. It implements the semantic rules of RPAL, managing environments, control stacks, and closures to execute the program.
Functionality: The CSE machine simulates the execution of the program, producing the final result as output.
Implementation: The CSE machine is designed as a stack-based virtual machine, processing instructions in a linear fashion according to the RPAL semantics.
