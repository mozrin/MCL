# Nirvana MVI - A Simple Interpreted Language

## Introduction

Nirvana MVI (Minimum Viable Interpreter) is a small, custom-built interpreted language implemented in C++. This project serves as a foundational exploration into the core components of an interpreted language, from raw source code to executed output.

## Motivation

The primary motivation behind this project is to provide a hands-on, advanced learning exercise to keep C++ skills sharp and deepen understanding of the construction of programming languages. This is not intended as a production-ready language, but rather a deliberate and focused educational endeavor. All contributions, ideas, and questions are welcome.

## AI Collaboration

This project extensively leverages advanced AI models in its development process. I use, almost exclusively, pre-release versions of Google's Gemini LLMs via the [Google AI Studio](https://aistudio.google.com/).

The `prep_ai` folder within the project structure contains prompt and problem files. These are used to define the core objectives of the project (in `prompt` files) and the current work focus (in `problem` files) respectively. I engage in a continuous feedback loop with the AI, reviewing, massaging, and cooperatively defining this project one step at a time. This approach allows me to dive into complex topics like interpreted computer languages and rapidly bring myself up to speed with a tool like C++ (which I haven't used commercially in over a decade) without the initial struggle. This process works uniquely for my brain, and while it may or may not work for yours, you are absolutely welcome to try it!

A significant advantage of using Google Gemini LLMs for this type of development is their ability to process many megabytes of data. This allows for comprehensive context sharing with the AI. Furthermore, the capability to adjust the "thinking" cost of the models enables fine-tuning for accuracy and depth in results – just look for one of their "thinking" LLMs!

## Development Environment

This project, along with all my other development endeavors, is proudly built and maintained within the streamlined workflow provided by the **DevLite Devcontainer**.

DevLite offers a lightweight, standardized, and portable development environment using Docker containers, ensuring consistent toolchains and dependencies across different machines. This significantly simplifies setup and collaboration.

You can learn more about DevLite Devcontainers and how to integrate them into your own projects here:

[http://github.com/moztopia/devlite](http://github.com/moztopia/devlite)

## Current Features

Nirvana MVI is currently capable of parsing and executing a subset of common programming constructs. It processes source code in several distinct stages:

### 1. Lexical Analysis (Lexer)

The Lexer is responsible for converting the raw source code string into a stream of meaningful tokens. It handles:

- **Token Types:** Identifiers (variables), string literals, number literals, boolean literals (`true`, `false`), and various operators and punctuation (semicolon, dot, equal, parentheses, arithmetic, comparison, logical, unary).
- **Keywords:** Recognizes reserved words like `echo`, `and`, `or`, `not`.
- **Whitespace and Comments:** Skips over spaces, tabs, newlines, and supports both single-line (`//`) and multi-line (`/* ... */`) comments.
- **Error Reporting:** Detects and reports unknown characters and unterminated string literals, including the line number where the error occurred.

### 2. Syntactic Analysis (Parser)

The Parser takes the stream of tokens from the Lexer and constructs an Abstract Syntax Tree (AST). This tree is a hierarchical representation of the program's structure, reflecting the grammatical rules of the language. It enforces:

- **Statements:** Currently supports variable assignment (`$variable = expression;`) and `echo` statements (`echo expression;`).
- **Expressions:** Handles complex expressions involving:
  - **Primary Expressions:** String literals, number literals, boolean literals, variables, and parenthesized expressions for grouping.
  - **Unary Operators:** Logical negation (`not`, `!`) and numeric negation (`-`).
  - **Arithmetic Operations:** Multiplication (`*`), division (`/`), addition (`+`), subtraction (`-`), with standard operator precedence.
  - **Comparison Operators:** Greater than (`>`), greater than or equal to (`>=`), less than (`<`), less or equal to (`<=`), equality (`==`), inequality (`!=`).
  - **Logical Operators:** Logical AND (`and`), logical OR (`or`).
  - **String Concatenation:** Uses the dot operator (`.`) for joining strings and converting other types to strings for concatenation.
- **Error Reporting:** Identifies syntax errors (e.g., missing semicolons, unexpected tokens) and reports them along with the line number.

### 3. Abstract Syntax Tree (AST)

The AST is the in-memory representation of the parsed program. It consists of various node types, each representing a specific language construct (e.g., `ProgramNode`, `AssignmentStatement`, `EchoStatement`, `BinaryOpExpr`, `StringLiteralExpr`, `NumberLiteralExpr`, `BooleanLiteralExpr`, `VariableExpr`, `UnaryOpExpr`). This structured representation is then traversed by the evaluator.

### 4. Evaluation (Evaluator)

The Evaluator is the interpreter's runtime component. It traverses the AST and executes the program logic:

- **Variable Management:** Maintains a symbol table (`std::map<std::string, Value>`) to store and retrieve variable values.
- **Expression Evaluation:** Performs arithmetic, logical, comparison, and string operations based on the AST structure.
- **Type System:** Employs a `std::variant` (`Value`) to dynamically handle different data types (string, double, bool, monostate for null/void).
- **Type Coercion:** Supports automatic type conversion for string concatenation (e.g., `123 . "hello"` becomes `"123hello"`).
- **Runtime Errors:** Catches and reports runtime issues such as undefined variables, division by zero, or type mismatches for specific operations.
- **Output:** Processes `echo` statements to print expression results to standard output.

## How to Run

To compile and run Nirvana MVI, navigate to the project's root directory (e.g., `/nirvana/code/`) in your terminal and use a C++17 compatible compiler (like g++):

```bash
g++ -std=c++17 -g src/*.cpp -Iinclude -o bin/nirvana
```

Then, execute the interpreter with the example script:

```bash
./bin/nirvana bin/main.nv
```

## Contributing

You are always welcome to ask anything about this project. If I can tell you the answer or help you, I will. Feel free to explore, modify, and extend this project. I would appreciate at the very least a fork if you plan to build upon it!
