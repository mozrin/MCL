# Function: pi

Calculates and returns the mathematical constant Pi (π) to a specified number of significant places after the decimal point.

## Signature

`pi(integer $significant_places = MCL_PI_PRECISION_DEFAULT): number`

## Parameters

*   `$significant_places` (optional): An `integer` representing the number of digits to return after the decimal point.
    *   **Default Value**: `MCL_PI_PRECISION_DEFAULT` (which is `14`).
    *   **Valid Range**: Must be an integer between `0` and `MCL_PI_PRECISION_DEFAULT` (inclusive).
        *   If `0` is specified, the function returns the integer part of Pi (i.e., `3`).

## Returns

Returns a `number` (double) value representing Pi, truncated to the specified number of decimal places. The maximum precision is limited by the underlying `number` type's capabilities (typically 14-17 decimal digits for standard doubles).

## Examples

```mcl
echo pi();         # Output: 3.14159265358979
echo pi(0);        # Output: 3
echo pi(5);        # Output: 3.14159
echo pi(14);       # Output: 3.14159265358979
```

## Errors

*   Throws a runtime error if `$significant_places` is not an `integer` or `number` convertible to an `integer`.
*   Throws a runtime error if `$significant_places` is outside the valid range (i.e., less than `0` or greater than `MCL_PI_PRECISION_DEFAULT`).
*   Throws a runtime error if more than one argument is provided.