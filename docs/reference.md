# MCL Language Reference

This document provides a comprehensive reference for the MCL (My Custom Language) programming language, detailing its lexical structure, data types, variables, expressions, statements, built-in functions (extensions), and constants. MCL is an interpreted language designed with simplicity and extensibility in mind.

## 1. Introduction

MCL is a dynamically typed, procedural language that supports basic data types, arithmetic and logical operations, string manipulation, and a set of useful built-in functions. Its design emphasizes clear syntax and straightforward execution flow.

## 2. Lexical Structure

The lexical structure defines the basic building blocks of the MCL language, such as how comments are written, how identifiers are formed, and the representation of literals and operators.

### 2.1. Comments

MCL supports three types of comments to annotate code:

*   **Single-line comments (C++ style)**: Start with `//`. All text from `//` to the end of the line is considered a comment and ignored by the interpreter.
    ```mcl
    // This is a single-line comment.
    echo "Hello World"; // This comment is after a statement.
    ```
*   **Multi-line comments (C style)**: Enclosed within `/*` and `*/`. All text between these delimiters, across multiple lines, is treated as a comment.
    ```mcl
    /*
    This is a multi-line comment.
    It can span several lines.
    */
    echo "Multi-line comment example";
    ```
*   **Single-line comments (Shell script style)**: Start with `#`. Similar to `//`, all text from `#` to the end of the line is a comment.
    ```mcl
    # This is another single-line comment style.
    echo "Another comment example";
    ```

### 2.2. Identifiers

Identifiers in MCL are used to name variables and functions. An identifier must start with a dollar sign (`$`), followed by one or more alphanumeric characters (letters `a-z`, `A-Z`, numbers `0-9`), underscores (`_`), or hyphens (`-`). Keywords are reserved and cannot be used as identifiers.

**Examples:**
*   `$myVariable`
*   `$another_variable`
*   `$count-down`

### 2.3. Keywords

Keywords are reserved words in MCL that have special meaning and cannot be used as identifiers.

*   `echo`
*   `true`
*   `false`
*   `and`
*   `or`
*   `not`
*   `public`
*   `string`
*   `integer`
*   `number`
*   `boolean`

### 2.4. Literals

Literals represent fixed values directly in the source code. MCL supports string, number, and boolean literals.

#### 2.4.1. String Literals

String literals are sequences of characters enclosed within double quotes (`"`). They can include various escape sequences:

*   `\"`: Double quote
*   `\\`: Backslash
*   `\n`: Newline character
*   `\t`: Tab character

**Examples:**
*   `"Hello, MCL!"`
*   `"This is a string with a new line\nand a tab\tcharacter."`
*   `"Path to file: C:\\Program Files\\App"`

#### 2.4.2. Number Literals

Number literals represent numeric values. MCL distinguishes between integers and floating-point numbers.

*   **Integers**: Whole numbers without a decimal point. They are internally represented as 64-bit signed integers (`long long` in C++).
    **Examples:** `123`, `0`, `-45`, `9876543210`
*   **Floating-point Numbers**: Numbers with a decimal point. They are internally represented as double-precision floating-point numbers (`double` in C++).
    **Examples:** `3.14`, `-0.5`, `1.0`, `0.001`

#### 2.4.3. Boolean Literals

Boolean literals represent truth values.

*   `true`: Represents the boolean true value.
*   `false`: Represents the boolean false value.

### 2.5. Operators and Punctuation

MCL uses various symbols for operations and to structure the code.

*   `;`: Statement terminator (semicolon)
*   `.`: String concatenation
*   `=`: Assignment operator
*   `(`: Left parenthesis (for expressions and function calls)
*   `)`: Right parenthesis (for expressions and function calls)
*   `+`: Addition
*   `-`: Subtraction (also unary negation)
*   `*`: Multiplication
*   `/`: Division
*   `!`: Logical NOT (unary)
*   `<`: Less than
*   `>`: Greater than
*   `!=`: Not equal
*   `==`: Equal
*   `<=`: Less than or equal to
*   `>=`: Greater than or equal to
*   `,`: Argument separator in function calls
*   `|`: Bitwise OR

## 3. Data Types

MCL supports the following fundamental data types, inferred dynamically or explicitly declared for variables:

*   **`string`**: Represents textual data.
*   **`integer`**: Represents whole numbers, stored as 64-bit signed integers.
*   **`number`**: Represents floating-point numbers, stored as double-precision floating-point numbers.
*   **`boolean`**: Represents truth values (`true` or `false`).
*   **`null`**: A special value indicating the absence of a meaningful value. This is represented by `std::monostate` internally.

### 3.1. Type Coercion and Enforcement

MCL performs implicit type coercion in certain contexts (e.g., arithmetic operations, string concatenation, logical operations). However, when a variable is explicitly declared with a type, MCL enforces strict type checking:

*   If an `integer` variable is assigned a `number` (double), the `number` must be a whole number that can be exactly represented as a `long long`.
*   If a `boolean` variable is assigned an `integer`, the `integer` must be `0` (for `false`) or `1` (for `true`).
*   Assigning values of completely incompatible types (e.g., a `string` to an `integer` variable) will result in a runtime error.

## 4. Variables

Variables are named storage locations for values. In MCL, variables are identified by a leading dollar sign (`$`).

### 4.1. Declaration

Variables can be explicitly declared using the `public` keyword followed by a type keyword (`string`, `integer`, `number`, `boolean`), the variable's identifier, and optionally an initial value. All declarations must end with a semicolon (`;`).

**Syntax:**
```
public <type> $<identifier> = <expression>;
public <type> $<identifier>;
```

**Examples:**
```mcl
public integer $age = 30;
public string $name = "Alice";
public boolean $isActive; // Default value is 'null' (monostate)
public number $pi = 3.14159;
```

### 4.2. Assignment

Values can be assigned to variables using the assignment operator (`=`). If a variable is already declared, its type is respected during assignment. If a variable is assigned to without prior declaration, it is implicitly declared with a dynamic type (type `ANY`), meaning it can later hold values of any type.

**Syntax:**
```
$<identifier> = <expression>;
```

**Examples:**
```mcl
$age = $age + 1; // Update existing variable
$message = "Hello, MCL!"; // Implicit declaration and assignment
$isActive = true;
$pi = 3.14; // Reassignment
```

## 5. Expressions

Expressions combine literals, variables, and operators to produce a value.

### 5.1. Operator Precedence

MCL follows standard operator precedence rules, determining the order in which operations are performed in an expression. Parentheses can always be used to override default precedence.

1.  **Primary Expressions**: Literals (`"abc"`, `123`, `3.14`, `true`, `false`), variable references (`$myVar`), parenthesized expressions (`(expr)`).
2.  **Function Calls**: `functionName(arg1, arg2, ...)`
3.  **Unary Operators**: `-` (negation), `!` (logical NOT), `not` (logical NOT).
4.  **Multiplication/Division**: `*`, `/` (left-associative).
5.  **Addition/Subtraction**: `+`, `-` (left-associative).
6.  **Comparison Operators**: `>`, `>=`, `<`, `<=` (left-associative).
7.  **Equality Operators**: `==`, `!=` (left-associative).
8.  **Bitwise OR**: `|` (left-associative).
9.  **Logical AND**: `and` (left-associative).
10. **Logical OR**: `or` (left-associative).
11. **Concatenation**: `.` (left-associative).

### 5.2. Arithmetic Operators

Arithmetic operators perform mathematical calculations on numeric values. Boolean values are implicitly converted to `integer` (0 for `false`, 1 for `true`) for arithmetic operations.

*   `+` (Addition): `$x + $y`
*   `-` (Subtraction): `$x - $y`
*   `*` (Multiplication): `$x * $y`
*   `/` (Division): `$x / $y`
    *   Integer division (`long long / long long`) truncates towards zero.
    *   Floating-point division (`double / double` or `long long / double` or `double / long long`) produces a floating-point result.
    *   Division by zero for integers or floating-point numbers results in a runtime error.

### 5.3. Concatenation Operator (`.`)

The dot operator (`.`) concatenates values into a single string. All operands are converted to their string representation before concatenation.

**Example:**
```mcl
echo "The answer is " . 42 . "."; // Output: The answer is 42.
echo true . " and " . false;     // Output: true and false```

### 5.4. Comparison Operators

Comparison operators compare two values and return a `boolean` result (`true` or `false`). Numeric comparison is performed for `integer`, `number`, and `boolean` types (booleans are treated as 0 or 1).

*   `>` (Greater than): `$x > $y`
*   `>=` (Greater than or equal to): `$x >= $y`
*   `<` (Less than): `$x < $y`
*   `<=` (Less than or equal to): `$x <= $y`

**Example:**
```mcl
echo 10 > 5;   // Output: true
echo 3.14 <= 3.14; // Output: true
```

### 5.5. Equality Operators

Equality operators compare two values for equality or inequality and return a `boolean` result.

*   `==` (Equal): `$x == $y`
*   `!=` (Not Equal): `$x != $y`

**Comparison Rules:**
*   **Numeric Types (`integer`, `number`, `boolean`)**: Compared numerically. If types differ (e.g., `integer` and `number`), both are converted to `double` for comparison. A small epsilon is used for `double` comparisons to account for floating-point inaccuracies.
*   **String Types**: Compared lexicographically.
*   **Mixed Types (non-numeric)**: Generally considered not equal unless specifically handled (e.g., numeric coercion).
*   **`null`**: `null` is only equal to `null`.

**Examples:**
```mcl
echo 10 == 10.0;   // Output: true
echo "hello" == "world"; // Output: false
echo true == 1;    // Output: true
echo false != 0;   // Output: false
```

### 5.6. Logical Operators

Logical operators combine boolean expressions and return a `boolean` result. Non-boolean values are coerced to boolean: `false` if empty string, `0`, `0.0`, or `null`; otherwise `true`. MCL's logical operators are short-circuiting.

*   `and`: Logical AND. Returns `true` if both operands are `true`.
*   `or`: Logical OR. Returns `true` if at least one operand is `true`.
*   `!` (Logical NOT): Unary operator. Returns the opposite boolean value of its operand.
*   `not` (Logical NOT): Unary operator. Same as `!`.

**Examples:**
```mcl
echo (true and false);   // Output: false
echo (true or false);    // Output: true
echo !true;              // Output: false
echo not ("");           // Output: true (empty string is false)
```

### 5.7. Bitwise OR Operator (`|`)

The bitwise OR operator (`|`) performs a bitwise OR operation on its operands. This operator requires integer-like operands.

*   Operands must be `integer` or `number` values that can be exactly converted to an `integer`. `boolean` values are treated as `0` or `1`.
*   A runtime error occurs if an operand is a floating-point number that is not a whole number.

**Example:**
```mcl
echo 5 | 3;  // Binary: 0101 | 0011 = 0111 (Decimal 7). Output: 7
```

### 5.8. Unary Operators

Unary operators operate on a single operand.

*   `-` (Negation): Negates a numeric value.
    *   Applied to `integer` or `boolean`: returns `integer`.
    *   Applied to `number`: returns `number`.
    **Example:** `echo -10; // Output: -10`
*   `!` (Logical NOT) / `not` (Logical NOT): See Logical Operators (Section 5.6).

### 5.9. Function Calls

MCL supports calling built-in functions (extensions and helpers) using a standard function call syntax: the function name followed by a list of arguments enclosed in parentheses and separated by commas.

**Syntax:**
```
functionName(arg1, arg2, ...);
```

**Example:**
```mcl
echo abs(-10);
public string $text = " hello world ";
echo trim($text);
```

## 6. Statements

Statements are instructions that perform actions. MCL currently supports variable declaration, assignment, and the `echo` statement. All statements must be terminated by a semicolon (`;`).

### 6.1. `echo` Statement

The `echo` statement is used to print the value of an expression to the standard output.

**Syntax:**
```
echo <expression>;
```

The expression is evaluated, converted to its string representation, and printed to the console, followed by a newline character.

**Examples:**
```mcl
echo "Hello, MCL!";
echo 10 + 5;
echo $myVar . " is the value.";
```

## 7. Built-in Functions (Extensions)

MCL provides a rich set of built-in functions, known as "extensions," to perform common operations.

### 7.1. `abs(number $value)`

*   **Description**: Returns the absolute value of the provided number.
*   **Arguments**:
    *   `$value` (`integer` or `number`): The number whose absolute value is to be calculated.
*   **Returns**: An `integer` if the input was an `integer`, otherwise a `number`.
*   **Errors**: Throws a runtime error if `$value` is not a `number` type.
*   **Example**: `echo abs(-10); // Output: 10`

### 7.2. `ceiling(number $value)`

*   **Description**: Returns the smallest integer value greater than or equal to `value`.
*   **Arguments**:
    *   `$value` (`integer` or `number`): The number to ceil.
*   **Returns**: A `number` representing the ceiling value (even if it's a whole number).
*   **Errors**: Throws a runtime error if `$value` is not a `number` type.
*   **Example**: `echo ceiling(3.14); // Output: 4`

### 7.3. `echo(...$values)`

*   **Description**: Prints all provided arguments to the standard output, separated by spaces, followed by a newline. This is a function, distinct from the `echo` statement.
*   **Arguments**: `...$values` (any number of arguments of any type).
*   **Returns**: `null`.
*   **Errors**: Throws a runtime error if no arguments are provided.
*   **Example**: `echo("Hello", "World", 123, true); // Output: Hello World 123 true`

### 7.4. `floor(number $value)`

*   **Description**: Returns the largest integer value less than or equal to `value`.
*   **Arguments**:
    *   `$value` (`integer` or `number`): The number to floor.
*   **Returns**: A `number` representing the floor value (even if it's a whole number).
*   **Errors**: Throws a runtime error if `$value` is not a `number` type.
*   **Example**: `echo floor(3.99); // Output: 3`

### 7.5. `max(number ...$values)`

*   **Description**: Returns the largest value among all provided numeric arguments.
*   **Arguments**: `...$values` (one or more `integer` or `number` arguments).
*   **Returns**: An `integer` or `number`, preserving the type of the argument that was determined to be the maximum if possible, otherwise `number`.
*   **Errors**: Throws a runtime error if no arguments are provided or if any argument is not a `number` type.
*   **Example**: `echo max(10, 20, 5, 15); // Output: 20`

### 7.6. `min(number ...$values)`

*   **Description**: Returns the smallest value among all provided numeric arguments.
*   **Arguments**: `...$values` (one or more `integer` or `number` arguments).
*   **Returns**: An `integer` or `number`, preserving the type of the argument that was determined to be the minimum if possible, otherwise `number`.
*   **Errors**: Throws a runtime error if no arguments are provided or if any argument is not a `number` type.
*   **Example**: `echo min(10, 20, 5, 15); // Output: 5`

### 7.7. `sqrt(number $value)`

*   **Description**: Returns the square root of `value`.
*   **Arguments**:
    *   `$value` (`integer` or `number`): The non-negative number.
*   **Returns**: A `number`.
*   **Errors**: Throws a runtime error if `$value` is not a `number` type or is negative.
*   **Example**: `echo sqrt(25); // Output: 5`

### 7.8. `trim(string $str, string $characters, integer $operation)`

*   **Description**: Removes leading, trailing, or internal specified characters from a string.
*   **Arguments**:
    *   `$str` (`string`): The input string to trim.
    *   `$characters` (`string`, optional): A string containing all characters to be trimmed. Defaults to `MCL_TRIM_CHARS_DEFAULT` (whitespace characters).
    *   `$operation` (`integer`, optional): An integer constant specifying the trimming behavior. Defaults to `MCL_TRIM_ENDS`.
*   **Returns**: A `string`.
*   **Errors**: Throws a runtime error if arguments are of incorrect types or if `$operation` is an invalid constant.
*   **Operation Constants (can be combined using `|` for `MCL_TRIM_ENDS` and `MCL_TRIM_ALL`)**:
    *   `MCL_TRIM_LEFT` (1): Removes characters from the beginning of the string.
    *   `MCL_TRIM_RIGHT` (2): Removes characters from the end of the string.
    *   `MCL_TRIM_ENDS` (3): Removes characters from both ends (equivalent to `MCL_TRIM_LEFT | MCL_TRIM_RIGHT`).
    *   `MCL_TRIM_MIDDLE` (4): Reduces consecutive occurrences of trim characters within the string to a single instance.
    *   `MCL_TRIM_ALL` (7): Combines `MCL_TRIM_LEFT`, `MCL_TRIM_RIGHT`, and `MCL_TRIM_MIDDLE`.
*   **Example**:
    ```mcl
    echo trim("  hello world  ");                         // Output: "hello world"
    echo trim("---hello---", "-");                       // Output: "hello"
    echo trim("---hello---", "-", MCL_TRIM_LEFT);        // Output: "hello---"
    echo trim("h---e---l---l---o", "-", MCL_TRIM_MIDDLE); // Output: "h-e-l-l-o"
    ```

### 7.9. `uppercase(string $value, integer $technique)`

*   **Description**: Converts a string to uppercase based on the specified technique.
*   **Arguments**:
    *   `$value` (`string`): The input string.
    *   `$technique` (`integer`, optional): An integer constant defining the uppercase conversion method. Defaults to `MCL_UPPERCASE_EVERYTHING`.
*   **Returns**: A `string`.
*   **Errors**: Throws a runtime error if arguments are of incorrect types or if `$technique` is an invalid constant.
*   **Technique Constants**:
    *   `MCL_UPPERCASE_EVERYTHING` (1): Converts every character to uppercase.
    *   `MCL_UPPERCASE_TITLE` (2): Converts the first character of each word to uppercase, and all other characters to lowercase.
    *   `MCL_UPPERCASE_FIRST` (3): Converts only the very first alphabetical character of the string to uppercase; leaves others unchanged.
    *   `MCL_UPPERCASE_ALTERNATING` (4): Alternates the case of alphabetical characters (e.g., `aBcDeFg`). Non-alphabetical characters are ignored.
    *   `MCL_UPPERCASE_TOGGLE` (5): Toggles the case of each alphabetical character (uppercase becomes lowercase, lowercase becomes uppercase).
*   **Example**:
    ```mcl
    echo uppercase("hello world");                    // Output: HELLO WORLD
    echo uppercase("hello world", MCL_UPPERCASE_TITLE); // Output: Hello World
    echo uppercase("hELLo wORLd", MCL_UPPERCASE_TOGGLE); // Output: HellO WorLd
    ```

### 7.10. `wrap(string $value, string $characters)`

*   **Description**: Wraps a string with specified characters.
*   **Arguments**:
    *   `$value` (`string`): The input string to be wrapped.
    *   `$characters` (`string`, optional): A string defining the wrapping characters.
        *   If `|` is **not** present, the entire `$characters` string is used as both the prefix and suffix.
        *   If `|` **is** present, the portion before `|` is used as the prefix, and the portion after `|` is used as the suffix.
        Defaults to an empty string (`""`), resulting in no wrapping.
*   **Returns**: A `string`.
*   **Errors**: Throws a runtime error if arguments are of incorrect types.
*   **Example**:
    ```mcl
    echo wrap("text", "[ ]");             // Output: [ ]text[ ]
    echo wrap("text", "[" | "]");          // Output: [text]
    echo wrap("quoted", MCL_WRAP_CHARS_DEFAULT); // Output: 'quoted' (using MCL_WRAP_CHARS_DEFAULT: "'|'")
    ```

## 8. Constants

MCL provides several pre-defined constants for use with built-in functions, particularly for `trim` and `uppercase` operations. These constants are globally available and can be used directly in expressions.

### 8.1. Uppercase Constants

*   `MCL_UPPERCASE_EVERYTHING`: (integer, value: `1`) - Used with `uppercase()` to convert all characters to uppercase.
*   `MCL_UPPERCASE_TITLE`: (integer, value: `2`) - Used with `uppercase()` for title case conversion.
*   `MCL_UPPERCASE_FIRST`: (integer, value: `3`) - Used with `uppercase()` to uppercase only the first character.
*   `MCL_UPPERCASE_ALTERNATING`: (integer, value: `4`) - Used with `uppercase()` for alternating case.
*   `MCL_UPPERCASE_TOGGLE`: (integer, value: `5`) - Used with `uppercase()` to toggle the case of characters.

### 8.2. Trim Constants

*   `MCL_TRIM_LEFT`: (integer, value: `1`) - Used with `trim()` to remove leading characters.
*   `MCL_TRIM_RIGHT`: (integer, value: `2`) - Used with `trim()` to remove trailing characters.
*   `MCL_TRIM_MIDDLE`: (integer, value: `4`) - Used with `trim()` to reduce consecutive characters in the middle.
*   `MCL_TRIM_ENDS`: (integer, value: `3`) - Used with `trim()` to remove characters from both ends (combines `MCL_TRIM_LEFT` and `MCL_TRIM_RIGHT`).
*   `MCL_TRIM_ALL`: (integer, value: `7`) - Used with `trim()` to apply all trimming operations (combines `MCL_TRIM_LEFT`, `MCL_TRIM_RIGHT`, and `MCL_TRIM_MIDDLE`).
*   `MCL_TRIM_CHARS_DEFAULT`: (string, value: `" \t\n\r\0\x0B"`) - The default set of characters trimmed by `trim()` if not specified.

### 8.3. Wrap Constants

*   `MCL_WRAP_CHARS_DEFAULT`: (string, value: `"'|'"`) - Default wrapper string for `wrap()` to produce single quotes.
*   `MCL_WRAP_CHARS_HASHES`: (string, value: `"#|#"`) - Wrapper string for `wrap()` to produce hash symbols.
*   `MCL_WRAP_CHARS_PIPES`: (string, value: `"\\||\\|"`) - Wrapper string for `wrap()` to produce escaped pipe symbols.

## 9. Helpers

MCL includes a mechanism for defining "helpers" – custom functions that act as aliases or wrappers around existing built-in extension functions. Helpers are configured externally via `.config` files and dynamically loaded by the interpreter.

The helper mechanism allows for:

*   **Renaming**: Giving a more convenient name to an existing extension function.
*   **Argument Reordering/Defaults**: Mapping helper arguments to extension arguments in a specific order, and providing default values for optional arguments or fixed values for certain arguments.
*   **Simplified Interfaces**: Creating simpler function signatures for complex underlying extension functions.

Each helper definition specifies:

*   Its `name` (how it's called in MCL code).
*   The `extension` function it maps to.
*   A sequence of `parameter` definitions:
    *   **Required parameters**: Marked with `<type>` (e.g., `<string>`, `<integer>`, `<number>`, `<boolean>`), indicating the expected type for a mandatory user-provided argument.
    *   **Optional parameters**: Marked with `[default_value]`, indicating an optional user-provided argument. If the user doesn't provide it, `default_value` (which can be a literal or a constant) is used.
    *   **Fixed parameters**: Simply a `value` (literal or constant), meaning this argument is always passed to the underlying extension function with this fixed value, and the user cannot provide it.

When a helper is called, the interpreter processes the user-provided arguments according to the helper's parameter definitions, constructs the appropriate argument list for the underlying extension, and then calls the extension. This provides a powerful way to extend MCL's functionality without modifying the core interpreter code.

**Example (Conceptual `src/helpers/system/log.config` for an imagined `log` helper):**

```
name=log
extension=echo
parameter_1=<string>
parameter_2=[MCL_LOG_LEVEL_INFO]
```
*(Note: `MCL_LOG_LEVEL_INFO` would be a hypothetical constant, and the `echo` extension would need to handle a second argument or be designed to accept arbitrary arguments, which it already does.)*

This would allow calling `log("My message");` or `log("Error", MCL_LOG_LEVEL_ERROR);`, which would then internally map to a call to the `echo` extension with adjusted arguments.
```