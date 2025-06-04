#include "debugger.h"
#include <iostream>
#include <string>
#include "../common/token.h"

#ifdef ENABLE_DEBUG_OUTPUT

void printIndent(int indent)
{
    for (int i = 0; i < indent; ++i)
    {
        std::cout << "  ";
    }
}

void debug_print_message(const std::string &msg)
{
    std::cout << msg << std::endl;
}

void debug_print_ast_header(const std::string &title)
{
    std::cout << "\n--- " << title << " ---" << std::endl;
}

void debug_print_ast_node(const ASTNode *node, int indent)
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
            debug_print_ast_node(stmt.get(), indent + 1);
        }
    }
    else if (auto *funcDecl = dynamic_cast<const FunctionDeclaration *>(node))
    {
        std::cout << "FunctionDeclaration (Name: " << funcDecl->name
                  << ", ReturnType: " << Token(funcDecl->return_type, "", 0).toString() << ")\n";
        printIndent(indent + 1);
        std::cout << "Parameters (" << funcDecl->parameters.size() << "):\n";
        for (const auto &param : funcDecl->parameters)
        {
            printIndent(indent + 2);
            std::cout << "Parameter (Type: " << Token(param.declared_type, "", 0).toString() << ", Name: " << param.name << ")\n";
            if (param.default_value)
            {
                printIndent(indent + 3);
                std::cout << "Default Value:\n";
                debug_print_ast_node(param.default_value.get(), indent + 4);
            }
        }
        printIndent(indent + 1);
        std::cout << "Body:\n";
        debug_print_ast_node(funcDecl->body.get(), indent + 2);
    }
    else if (auto *block = dynamic_cast<const BlockStatement *>(node))
    {
        std::cout << "BlockStatement (" << block->statements.size() << " statements)\n";
        for (const auto &stmt : block->statements)
        {
            debug_print_ast_node(stmt.get(), indent + 1);
        }
    }
    else if (auto *ret = dynamic_cast<const ReturnStatement *>(node))
    {
        std::cout << "ReturnStatement\n";
        if (ret->expression)
        {
            debug_print_ast_node(ret->expression.get(), indent + 1);
        }
        else
        {
            printIndent(indent + 1);
            std::cout << "No Return Value\n";
        }
    }
    else if (auto *decl = dynamic_cast<const DeclarationStatement *>(node))
    {
        std::cout << "DeclarationStatement (Type: " << Token(decl->declared_type, "", 0).toString() << ", Target: " << decl->target->name << ")\n";
        if (decl->value)
        {
            debug_print_ast_node(decl->value.get(), indent + 1);
        }
        else
        {
            printIndent(indent + 1);
            std::cout << "Initial Value: None\n";
        }
    }
    else if (auto *assign = dynamic_cast<const AssignmentStatement *>(node))
    {
        std::cout << "AssignmentStatement (Target: " << assign->target->name << ")\n";
        debug_print_ast_node(assign->value.get(), indent + 1);
    }
    else if (auto *echo = dynamic_cast<const EchoStatement *>(node))
    {
        std::cout << "EchoStatement\n";
        debug_print_ast_node(echo->expression.get(), indent + 1);
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
        debug_print_ast_node(binOp->left.get(), indent + 1);
        debug_print_ast_node(binOp->right.get(), indent + 1);
    }
    else if (auto *unaryOp = dynamic_cast<const UnaryOpExpr *>(node))
    {
        std::cout << "UnaryOpExpr (Op: " << Token(unaryOp->op, "", 0).toString() << ")\n";
        debug_print_ast_node(unaryOp->right.get(), indent + 1);
    }
    else if (auto *call = dynamic_cast<const CallExpr *>(node))
    {
        std::cout << "CallExpr\n";
        printIndent(indent + 1);
        std::cout << "Callee:\n";
        debug_print_ast_node(call->callee.get(), indent + 2);
        printIndent(indent + 1);
        std::cout << "Arguments (" << call->arguments.size() << "):\n";
        for (const auto &arg : call->arguments)
        {
            debug_print_ast_node(arg.get(), indent + 2);
        }
    }
    else
    {
        std::cout << "Unknown AST Node Type\n";
    }
}

void debug_print_ast_footer()
{
    std::cout << "----------------------------------\n\n";
}

#endif