#ifndef MCL_DEBUGGER_H
#define MCL_DEBUGGER_H

#include <string>
#include <memory>
#include "ast.h"

// To ENABLE debug output, uncomment the line below:
// #define ENABLE_DEBUG_OUTPUT

#ifdef ENABLE_DEBUG_OUTPUT

void debug_print_message(const std::string &msg);
void debug_print_ast_header(const std::string &title);
void debug_print_ast_node(const ASTNode *node, int indent = 0);
void debug_print_ast_footer();

#else

inline void debug_print_message(const std::string &msg) {}
inline void debug_print_ast_header(const std::string &title) {}
inline void debug_print_ast_node(const ASTNode *node, int indent = 0) {}
inline void debug_print_ast_footer() {}

#endif

#endif