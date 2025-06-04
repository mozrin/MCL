Below is an example of a complete Markdown reference document for your `trim` function. This document details the syntax, parameters (including optional ones), error handling, available system constants, and usage examples.

---

# `trim` Function Reference

The `trim` function is designed to remove unwanted characters from a string. It offers full control over which parts of the string—left, right, or even the middle—should be trimmed based on a set of characters, using defined system constants for operations.

---

## Syntax

```cpp
trim(string $str, string $characters = " \t\n\r\0\x0B", int $operation = MCL_TRIM_ENDS)
```

- **`$str`** (String):  
  The input string to be trimmed.  
  **Required**

- **`$characters`** (String):  
  A list of characters to remove from `$str`. The default is `" \t\n\r\0\x0B"`, which includes:
  - Space (` `)
  - Horizontal tab (`\t`)
  - Newline (`\n`)
  - Carriage return (`\r`)
  - Null byte (`\0`)
  - Vertical tab (`\x0B`)
  
  **Optional**

- **`$operation`** (Integer):  
  Indicates the trimming operation via system constants (or their combinations). Accepts an integer value defined by available constants.  
  **Optional** (Defaults to `MCL_TRIM_ENDS`)

---

## Available Operation Constants

Use the following constants with the `$operation` parameter to control which part(s) of the string are trimmed:

| Constant           | Value | Description                                                                                         |
|--------------------|-------|-----------------------------------------------------------------------------------------------------|
| `MCL_TRIM_LEFT`    | 1     | Trim characters from the **left** (beginning) of the string.                                       |
| `MCL_TRIM_RIGHT`   | 2     | Trim characters from the **right** (end) of the string.                                            |
| `MCL_TRIM_MIDDLE`  | 4     | Condense multiple consecutive occurrences of trim characters in the **middle** of the string to a single instance. |
| `MCL_TRIM_ENDS`    | 3     | **Default**: Combines left (1) and right (2) trimming (i.e., trim both ends).                      |
| `MCL_TRIM_ALL`     | 7     | Combines left (1), right (2), and middle (4) trimming—applies all operations.                        |

**Notes:**

- When no `$operation` is provided, `MCL_TRIM_ENDS` (3) is used.
- You may combine operations using bitwise OR. For example, combining left and middle trimming yields `MCL_TRIM_LEFT | MCL_TRIM_MIDDLE` (which equals `1 | 4 = 5`).

---

## Parameters Details

1. **`$str` (String)**  
   The base string from which characters will be trimmed.  
   - **Type Check:** Must be a string.  
   - **Error Handling:** If the provided value is not a string, the function throws a runtime error specifying that a string was expected along with the actual type provided.

2. **`$characters` (String)**  
   The set of characters that will be removed from `$str` according to the operation.  
   - **Default:** `" \t\n\r\0\x0B"`.  
   - **Usage:** The function checks within the input string for occurrences of any character in this set.  
   - **Error Handling:** If the supplied value isn’t a string, a runtime error is thrown with a descriptive message.

3. **`$operation` (Integer)**  
   Specifies which trimming operations to apply.  
   - **Acceptable Values:** Either as an integer or a number that represents an integer value.  
   - **Default:** `MCL_TRIM_ENDS` (3).  
   - **Valid Values:** 1, 2, 3, 4, 7 or any logical combination of the constants.  
   - **Special Check:** If a floating-point number is provided, it must be equivalent to an integer (i.e., no fractional part).  
   - **Error Handling:**  
     - If the argument isn’t an integer (or float representing an integer), the function throws a runtime error.
     - If an invalid operation constant (or combination) is provided, an error is thrown listing the valid options.

---

## Behavior

- **Left Trimming (`MCL_TRIM_LEFT`):**  
  The function locates the first character in `$str` that is not among `$characters` and returns the substring starting from that character onward.

- **Right Trimming (`MCL_TRIM_RIGHT`):**  
  The function locates the last character in `$str` that is not among `$characters` and returns the substring up to (and including) that character.

- **Middle Trimming (`MCL_TRIM_MIDDLE`):**  
  Beyond left- and right-trimming, if this flag is set, the function scans the string for consecutive occurrences of any of the `$characters` and reduces each group to a single occurrence.

- **Combined Operations:**  
  You can combine operations with bitwise OR. For example:
  - `MCL_TRIM_ENDS` (which is 1 | 2) trims both ends.
  - `MCL_TRIM_ALL` (7) trims both ends and condenses the middle.

- **Edge Cases:**  
  If, after trimming, no characters remain (i.e., the whole string consisted of trim characters), the function returns an empty string.

---

## Example Usage

### 1. Basic Trimming (Default Behavior)

```cpp
// Example: Trimming both ends (default operation: MCL_TRIM_ENDS).
string input = "   Hello World   ";
string output = trim(input); 
// Expected output: "Hello World"
```

### 2. Custom Character Set

```cpp
// Example: Removing '+' characters from both ends.
string input = "+++Hello+++";
string output = trim(input, "+");
// Expected output: "Hello"
```

### 3. Specifying Operation Constants

```cpp
// Example: Only left trimming.
string input1 = "   Hello World   ";
string leftTrimmed = trim(input1, " ", MCL_TRIM_LEFT);
// Expected output: "Hello World   "

// Example: Only right trimming.
string input2 = "   Hello World   ";
string rightTrimmed = trim(input2, " ", MCL_TRIM_RIGHT);
// Expected output: "   Hello World"

// Example: Trim both ends with left, right, and condense middle spaces.
string input3 = "  Hello    World  ";
string fullTrimmed = trim(input3, " ", MCL_TRIM_ALL);
// Expected output: "Hello World" (with a single space between "Hello" and "World")
```

---

## Registration in the Evaluator

To make the `trim` function available to your language interpreter or evaluator, register it using the provided extension function:

```cpp
void register_trim_extension(Evaluator &eval)
{
    eval.registerNativeFunction("trim", nirvana_trim);
}
```

---

## Conclusion

The `trim` function is a versatile tool for string manipulation in your language. Its robust parameter type-checking, detailed error messages, and flexible operation constants ensure precise control over text processing. Whether you need to simply remove whitespace from both ends or perform complex trimming operations, this function provides an elegant solution for handling strings.

---

### Additional Tips

- **Combining Operations:**  
  To experiment with different trimming behaviors, try combining constants using bitwise OR. For example, `MCL_TRIM_LEFT | MCL_TRIM_MIDDLE` will trim the left side and reduce duplicate trim characters in the middle.

- **Custom Character Sets:**  
  Using a custom set of characters can help in scenarios where you need to sanitize input—such as removing particular punctuation from user data.

- **Error Messages:**  
  The function throws detailed error messages to aid debugging. If you encounter a type error, check the type of your parameters against the expected types.