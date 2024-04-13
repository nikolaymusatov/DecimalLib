# DecimalLib

In this project my own decimal.h library was implemented using the C programming language.

The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default value of a Decimal is 0. The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors. The Decimal type does not eliminate the need for rounding. Rather, it minimizes errors due to rounding.

When the result of the division and multiplication is passed to the Round method, the result suffers no loss of precision.

A decimal number is a floating-point value that consists of a sign, a numeric value where each digit in the value ranges from 0 to 9, and a scaling factor that indicates the position of a floating decimal point that separates the integral and fractional parts of the numeric value.

The binary representation of a Decimal value consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the 96-bit integer and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28. Therefore, the binary representation of a Decimal value the form, ((-2^96 to 2^96) / 10^(0 to 28)), where -(2^96-1) is equal to MinValue, and 2^96-1 is equal to MaxValue.

The functions of the decimal.h library described below were implemented as:
- The library was developed in C language of C11 standard using gcc compiler
- The library code is located in the src folder   
- Build as a static library (with the decimal.h header file)
- The library is developed according to the principles of structured programming;
- Full coverage of library functions code with unit-tests using the Check library was prerared
- Makefile is provided for building the library and tests (with targets all, clean, test, decimal.a, gcov_report)  
- The gcov_report target generates a gcov report in the form of an html page.
- The defined type supports numbers from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335.

# Information
## Binary representation

The binary representation of a Decimal number consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the integer number and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28.

Decimal number is implemented as a four-element array of 32-bit signed integers (`int bits[4];`).

`bits[0]`, `bits[1]`, and `bits[2]` contain the low, middle, and high 32 bits of the 96-bit integer number accordingly.

`bits[3]` contains the scale factor and sign, and consists of following parts:
- Bits 0 to 15, the lower word, are unused and must be zero.
- Bits 16 to 23 must contain an exponent between 0 and 28, which indicates the power of 10 to divide the integer number.
- Bits 24 to 30 are unused and must be zero.
- Bit 31 contains the sign; 0 meaning positive, and 1 meaning negative.

## Arithmetic Operators

| Operator name | Operators  | Function                                                                           | 
| ------ | ------ |------------------------------------------------------------------------------------|
| Addition | + | int add(decimal value_1, decimal value_2, decimal *result)         |
| Subtraction | - | int sub( decimal value_1, decimal value_2, decimal *result) |
| Multiplication | * | int mul( decimal value_1, decimal value_2, decimal *result) | 
| Division | / | int div( decimal value_1, decimal value_2, decimal *result) |
| Modulo | Mod | int mod( decimal value_1, decimal value_2, decimal *result) |

The functions return the error code:
- 0 - OK
- 1 - the number is too large or equal to infinity
- 2 - the number is too small or equal to negative infinity
- 3 - division by 0

## Comparison Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Less than | < | int is_less(decimal, decimal) |
| Less than or equal to | <= | int is_less_or_equal(decimal, decimal) | 
| Greater than | > | int is_greater( decimal, decimal) |
| Greater than or equal to | >= | int is_greater_or_equal(decimal, decimal) | 
| Equal to | == | int is_equal(decimal, decimal) |
| Not equal to | != | int is_not_equal(decimal, decimal) |

Return value:
- 0 - FALSE
- 1 - TRUE

## Convertors and parsers

| Convertor/parser | Function | 
| ------ | ------ |
| From int | int from_int_to_decimal(int src, decimal *dst) |
| From float | int from_float_to_decimal(float src, decimal *dst) |
| To int | int from_decimal_to_int(decimal src, int *dst) |
| To float | int from_decimal_to_float(decimal src, float *dst) |

Return value - code error:
- 0 - OK
- 1 - convertation error

## Another functions

| Description | Function                                                         | 
| ------ |------------------------------------------------------------------|
| Rounds a specified Decimal number to the closest integer toward negative infinity. | int dec_floor(decimal value, decimal *result)            |	
| Rounds a decimal value to the nearest integer. | int dec_round( decimal value, decimal *result)    |
| Returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes. | int truncate( decimal value, decimal *result) |
| Returns the result of multiplying the specified Decimal value by negative one. | int negate(decimal value, decimal *result)   |

Return value - code error:
- 0 - OK
- 1 - calculation error