## `contains` Function

### Synopsis

`boolean contains(<string> $str, <string> $substring)`

### Description

The `contains` function checks if a given string (`$str`) includes a specific substring (`$substring`). The comparison is case-sensitive.

### Parameters

*   `$str` (`string`): The main string to search within.
*   `$substring` (`string`): The substring to search for.

### Return Value

Returns `true` if `$str` contains `$substring`, `false` otherwise.

### Errors

*   Throws a `Runtime Error` if the number of arguments is not exactly 2.
*   Throws a `Runtime Error` if either argument is not a `string`.

### Examples

```mcl
echo contains("hello world", "world"); // Output: true
echo contains("programming", "gram"); // Output: true
echo contains("apple", "orange"); // Output: false
echo contains("hello", "Hello"); // Output: false (case-sensitive)
echo contains("test", "test"); // Output: true
echo contains("abc", ""); // Output: true (empty string is contained in any string)
echo contains("", "a"); // Output: false
echo contains("", ""); // Output: true
public string $sentence = "The quick brown fox jumps over the lazy dog.";
public string $word = "fox";
public string $missing = "cat";
echo contains($sentence, $word); // Output: true
echo contains($sentence, $missing); // Output: false
```