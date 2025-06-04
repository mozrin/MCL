## `reverse` Function

### Synopsis

`string reverse(<string> $value1, [...<string> $valueN])`

### Description

The `reverse` function concatenates all provided string arguments and then reverses the order of the characters in the resulting combined string.

### Parameters

*   `$value1` (`string`): The first string to be reversed.
*   `...$valueN` (`string`, optional): Additional strings to be concatenated and then reversed.

### Return Value

Returns a `string` with the characters of the concatenated input strings in reverse order.

### Errors

*   Throws a `Runtime Error` if no arguments are provided.
*   Throws a `Runtime Error` if any argument is not a `string`.

### Examples

```
echo reverse("hello");                  // Output: olleh
echo reverse("world", "!");             // Output: !dlrow
echo reverse("madam", "I'm", "adam"); // Output: madama'I amdam
public string $str1 = "apple";
public string $str2 = "pie";
echo reverse($str1, $str2);             // Output: eippa
```