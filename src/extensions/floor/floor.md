## `floor` Function

### Synopsis

`number floor(<number> $value)`

### Description

The `floor` function rounds a given number down to the nearest whole number (integer).

If the argument is an integer, it returns the integer itself.
If the argument is a number (double), it returns the largest integer value that is less than or equal to the argument.

### Parameters

*   `$value` (`number`): The number to be rounded down. This can be an `integer` or a `number`.

### Return Value

Returns a `number` (specifically, a `double` if the input was a `double`, or a `long long` if the input was an `integer` that remains unchanged, though it will implicitly convert to `double` if the result requires it). The returned value represents the largest integer less than or equal to `$value`.

### Errors

*   Throws a `Runtime Error` if the number of arguments is not exactly 1.
*   Throws a `Runtime Error` if the argument is not a `number` (i.e., not an `integer` or `number` type).

### Examples

```
public number $num1 = 3.14;
public number $num2 = 3.99;
public integer $int1 = 5;
public number $negNum = -3.14;
public number $negInt = -5.0;

echo floor($num1);    // Output: 3
echo floor($num2);    // Output: 3
echo floor($int1);    // Output: 5
echo floor($negNum);  // Output: -4
echo floor($negInt);  // Output: -5
```