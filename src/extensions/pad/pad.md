## `pad` Function

### Synopsis

`string pad(<string> $str, <integer> $length, [<string> $pad_string = " "], [<integer> $pad_type = MCL_PAD_RIGHT])`

### Description

The `pad` function pads a string to a certain length with another string.

If the `length` argument is less than or equal to the length of the input string, no padding takes place, and the original string is returned.

The `pad_string` is truncated if the amount of padding needed makes it longer than the `pad_string` itself. If `pad_string` is an empty string, it defaults to a single space character.

The `pad_type` determines where the padding is applied:
*   `MCL_PAD_RIGHT` (default): Pads the string on the right side.
*   `MCL_PAD_LEFT`: Pads the string on the left side.

### Parameters

*   `$str` (`string`): The input string to pad.
*   `$length` (`integer`): The new length of the padded string.
*   `$pad_string` (`string`, optional): The string to use for padding. Defaults to a single space character (`" "`). If an empty string `""` is explicitly provided, it will also default to a single space character. Only the first character of `pad_string` is used for padding.
*   `$pad_type` (`integer`, optional): An integer constant specifying the padding direction. Defaults to `MCL_PAD_RIGHT`.
    *   `MCL_PAD_RIGHT` (value `1`)
    *   `MCL_PAD_LEFT` (value `2`)

### Return Value

Returns a `string` representing the padded version of the input string.

### Errors

*   Throws a `Runtime Error` if the number of arguments is less than 2 or greater than 4.
*   Throws a `Runtime Error` if `$str` is not a `string`.
*   Throws a `Runtime Error` if `$length` is not an `integer` (or a `number` convertible to an `integer`).
*   Throws a `Runtime Error` if `$pad_string` is provided and is not a `string`.
*   Throws a `Runtime Error` if `$pad_type` is provided and is not an `integer` (or a `number` convertible to an `integer`), or if its value is not `MCL_PAD_LEFT` or `MCL_PAD_RIGHT`.

### Examples

``` mcl
public string $str_hello = "hello";
echo pad($str_hello, 10);
// Output: "hello     "
echo pad($str_hello, 10, "-"); // Output: "hello-----"
echo pad($str_hello, 10, ".-");
// Output: "hello....." (uses only first char of ".-")
echo pad($str_hello, 10, "|", MCL_PAD_LEFT); // Output: "|||||hello"
echo pad($str_hello, 10, "_", MCL_PAD_RIGHT); // Output: "hello_____"
echo pad("test", 5); // Output: "test "
echo pad("longstring", 5); // Output: "longstring" (length <= original string length)
public string $str_empty = "";
echo pad($str_empty, 5, "#"); // Output: "#####"
echo pad($str_empty, 5, "", MCL_PAD_LEFT); // Output: "     " (empty pad_string defaults to space)
public string $long_pad = "XYZABC";
echo pad("short", 12, $long_pad); // Output: "shortXXXXXXX" (uses 'X' from 'XYZABC')
```