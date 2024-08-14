# Makefile for rpal_final

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -IOperations

# Source files and object files
SRCS := main.cpp Tree/tree.cpp Operations/biops.cpp
OBJS := $(SRCS:.cpp=.o)

# Header files
HDRS := Lexer.h Parser.h CSE.h Tokens/Token.h Tokens/TokenController.h Tree/TreeNode.h Tree/Tree.h Operations/biops.h 

# Target executable
TARGET := rpal20

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	del /Q *.o

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Header dependencies
$(OBJS): $(HDRS)

# Clean
clean:
	del /Q *.o rpal20.exe
