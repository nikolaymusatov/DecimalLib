#include "decimal.h"

int is_less(decimal value_1, decimal value_2) {
  int return_code = 0;
  int sign_1 = get_bit(value_1, 127);
  int sign_2 = get_bit(value_2, 127);
  if (is_bits_zero(value_1) && (is_bits_zero(value_2)))
    return_code = 0;
  else if (sign_1 != sign_2 && sign_1)
    return_code = 1;
  else if (sign_1 == sign_2) {
    long_decimal lvalue_1 = {{0}};
    long_decimal lvalue_2 = {{0}};
    get_equal_scale_long_decimal(value_1, value_2, &lvalue_1, &lvalue_2);
    if (!sign_1) return_code = long_is_bits_less(lvalue_1, lvalue_2);
    if (sign_1) return_code = long_is_bits_greater(lvalue_1, lvalue_2);
  }
  return return_code;
}

int is_less_or_equal(decimal value_1, decimal value_2) {
  int return_code = 0;
  if (is_less(value_1, value_2) || is_equal(value_1, value_2)) return_code = 1;
  return return_code;
}

int is_greater(decimal value_1, decimal value_2) {
  int return_code = 0;
  if (!is_less(value_1, value_2) && !is_equal(value_1, value_2))
    return_code = 1;
  return return_code;
}

int is_greater_or_equal(decimal value_1, decimal value_2) {
  int return_code = 0;
  if (!is_less(value_1, value_2)) return_code = 1;
  return return_code;
}

int is_equal(decimal value_1, decimal value_2) {
  int return_code = 0;
  int sign_1 = get_bit(value_1, 127);
  int sign_2 = get_bit(value_2, 127);
  if (is_bits_zero(value_1) && (is_bits_zero(value_2)))
    return_code = 1;
  else if (sign_1 == sign_2) {
    long_decimal lvalue_1 = {{0}};
    long_decimal lvalue_2 = {{0}};
    get_equal_scale_long_decimal(value_1, value_2, &lvalue_1, &lvalue_2);
    if (long_is_bits_equal(lvalue_1, lvalue_2)) return_code = 1;
  }
  return return_code;
}

int is_not_equal(decimal value_1, decimal value_2) {
  return !is_equal(value_1, value_2);
}
