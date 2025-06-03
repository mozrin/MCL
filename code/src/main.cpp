// /nirvana/prep_ai/../code/src/main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <exception>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "token.h"
#include "evaluator.h"

void printAST(const ASTNode *node, int indent = 0);

int main(int argc, char *argv[])
{
std::cout << "Nirvana MVI starting (Lexer + Parser + Evaluator)...\n";

std::string source_code;
std::string input_filename = "main.nv";

if (argc > 1)
{
input_filename = argv[1];
}

std::ifstream file(input_filename);
if (file.is_open())
{
std::stringstream buffer;
buffer << file.rdbuf();
source_code = buffer.str();
file.close();
}
else
{
std::cerr << "Error: Could not open file '" << input_filename << "'.\n";
std::cerr << "Please ensure '" << input_filename << "' exists in the project root,\n";
std::cerr << "or provide a valid path as a command-line argument.\n";
return 1;
}

Lexer lexer(source_code);
Parser parser(lexer);

std::cout << "Parsing file: '" << input_filename << "'...\n";

std::unique_ptr<ProgramNode> ast;
try
{
ast = parser.parseProgram();
std::cout << "Parsing finished.\n";
}
catch (const std::runtime_error &e)
{
std::cerr << "Parser/Lexer Error during parsing: " << e.what() << std::endl;
return 1;
}
catch (const std::exception &e)
{
std::cerr << "An unexpected error occurred during parsing: " << e.what() << std::endl;
return 1;
}

std::cout << "\n--- Abstract Syntax Tree (AST) ---" << std::endl;
printAST(ast.get());
std::cout << "----------------------------------\n\n";

std::cout << "Starting interpretation...\n";
Evaluator evaluator;
try
{
evaluator.interpret(std::move(ast));
std::cout << "Interpretation finished successfully.\n";
}
catch (const std::runtime_error &e)
{
std::cerr << "Runtime Error: " << e.what() << std::endl;
return 1;
}
catch (const std::exception &e)
{
std::cerr << "An unexpected error occurred during interpretation: " << e.what() << std::endl;
return 1;
}

std::cout << "AST generated. Ready for Evaluation (next step).\n";

return 0;
}

void printIndent(int indent)
{
for (int i = 0; i < indent; ++i)
{
std::cout << "  ";
}
}

void printAST(const ASTNode *node, int indent)
{
if (!node)
{
return;
}

printIndent(indent);

if (auto *prog = dynamic_cast<const ProgramNode *>(node))
{
std::cout << "ProgramNode (" << prog->statements.size() << " statements)\n";
for (const auto &stmt : prog->statements)
{
printAST(stmt.get(), indent + 1);
}
}
else if (auto *decl = dynamic_cast<const DeclarationStatement *>(node))
{
std::cout << "DeclarationStatement (Type: " << Token(decl->declared_type, "", 0).toString() << ", Target: " << decl->target->name << ")\n";
if (decl->value) {
    printAST(decl->value.get(), indent + 1);
} else {
    printIndent(indent + 1);
    std::cout << "Initial Value: None\n";
}
}
else if (auto *assign = dynamic_cast<const AssignmentStatement *>(node))
{
std::cout << "AssignmentStatement (Target: " << assign->target->name << ")\n";
printAST(assign->value.get(), indent + 1);
}
else if (auto *echo = dynamic_cast<const EchoStatement *>(node))
{
std::cout << "EchoStatement\n";
printAST(echo->expression.get(), indent + 1);
}
else if (auto *strLit = dynamic_cast<const StringLiteralExpr *>(node))
{
std::cout << "StringLiteralExpr: \"" << strLit->value << "\"\n";
}
else if (auto *numLit = dynamic_cast<const NumberLiteralExpr *>(node))
{
std::cout << "NumberLiteralExpr: " << numLit->value << "\n";
}
else if (auto *boolLit = dynamic_cast<const BooleanLiteralExpr *>(node))
{
std::cout << "BooleanLiteralExpr: " << (boolLit->value ? "true" : "false") << "\n";
}
else if (auto *var = dynamic_cast<const VariableExpr *>(node))
{
std::cout << "VariableExpr: " << var->name << "\n";
}
else if (auto *binOp = dynamic_cast<const BinaryOpExpr *>(node))
{
std::cout << "BinaryOpExpr (Op: " << Token(binOp->op, "", 0).toString() << ")\n";
printAST(binOp->left.get(), indent + 1);
printAST(binOp->right.get(), indent + 1);
}
else if (auto *unaryOp = dynamic_cast<const UnaryOpExpr *>(node))
{
std::cout << "UnaryOpExpr (Op: " << Token(unaryOp->op, "", 0).toString() << ")\n";
printAST(unaryOp->right.get(), indent + 1);
}
else
{
std::cout << "Unknown AST Node Type\n";
}
}