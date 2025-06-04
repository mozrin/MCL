## `min` Function

### Synopsis

`number min(<number> $value1, <number> $value2, [...<number> $valueN])`

### Description

The `min` function returns the smallest value among its arguments.
It accepts one or more arguments, all of which must be numbers (integers or floating-point numbers).
If all arguments are integers, the result will be an integer. If any argument is a floating-point number, the result will be a floating-point number, even if the minimum value itself is an integer.

### Parameters

*   `$value1` (`number`): The first number to compare.
*   `$value2` (`number`): The second number to compare.
*   `...$valueN` (`number`, optional): Additional numbers to compare.

### Return Value

Returns a `number` representing the minimum value found among the arguments. The type of the returned value (`integer` or `number`) depends on the types of the input arguments, promoting to `number` if any argument was a `number` (double).

### Errors

*   Throws a `Runtime Error` if no arguments are provided.
*   Throws a `Runtime Error` if any argument is not a `number` (i.e., not an `integer` or `number` type).

### Examples
Use code with caution.
```
echo min(5, 10); // Output: 5
echo min(3.14, 2.71); // Output: 2.71
echo min(10, 5, 20, 15); // Output: 5
echo min(-5, -10, -2); // Output: -10
echo min(1, 2.5, 0.5, 3); // Output: 0.5
echo min(5, 10.0, 7); // Output: 5.0 (because 10.0 is a number)
echo min(9.9, 10, 9.8); // Output: 9.8
echo min(7); // Output: 7
```
