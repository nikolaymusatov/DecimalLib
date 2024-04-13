#include "decimal.h"

int add(decimal value_1, decimal value_2, decimal *result) {
  long_decimal lvalue_1 = {{0}};
  long_decimal lvalue_2 = {{0}};
  long_decimal longres = {{0}};
  int sign_1 = get_bit(value_1, 127);
  int sign_2 = get_bit(value_2, 127);
  get_equal_scale_long_decimal(value_1, value_2, &lvalue_1, &lvalue_2);
  if (sign_1 == sign_2) {
    addition(lvalue_1, lvalue_2, &longres);
    long_set_bit(&longres, sign_1, 255);
  } else if (long_is_bits_greater(lvalue_1, lvalue_2)) {
    subtraction(lvalue_1, lvalue_2, &longres);
    long_set_bit(&longres, sign_1, 255);
  } else {
    subtraction(lvalue_2, lvalue_1, &longres);
    long_set_bit(&longres, sign_2, 255);
  }
  long_reduce_scale(&longres);
  return from_ldecimal_to_decimal(longres, result);
}

int sub(decimal value_1, decimal value_2, decimal *result) {
  negate(value_2, &value_2);
  return add(value_1, value_2, result);
}

int mul(decimal value_1, decimal value_2, decimal *result) {
  long_decimal lvalue_1 = {{0}};
  long_decimal lvalue_2 = {{0}};
  long_decimal longres = {{0}};
  int sign = get_bit(value_1, 127) != get_bit(value_2, 127);
  get_equal_scale_long_decimal(value_1, value_2, &lvalue_1, &lvalue_2);
  multiplication(lvalue_1, lvalue_2, &longres);
  long_set_bit(&longres, sign, 255);
  long_reduce_scale(&longres);
  return from_ldecimal_to_decimal(longres, result);
}

int div(decimal value_1, decimal value_2, decimal *result) {
  int return_code = 0;
  if (is_bits_zero(value_2))
    return_code = 3;
  else {
    long_decimal lvalue_1 = {{0}};
    long_decimal lvalue_2 = {{0}};
    long_decimal longres = {{0}};
    long_decimal mod = {{0}};
    decimal modres = {{0}};
    get_equal_scale_long_decimal(value_1, value_2, &lvalue_1, &lvalue_2);
    int_division(lvalue_1, lvalue_2, &longres, &mod);
    long_reduce_scale(&longres);
    return_code = from_ldecimal_to_decimal(longres, result);
    if (!long_is_bits_zero(mod)) {
      mod_division(mod, lvalue_2, &modres);
      add(modres, *result, result);
    }
  }
  return return_code;
}

int mod(decimal value_1, decimal value_2, decimal *result) {
  int return_code = 0;
  if (is_bits_zero(value_2))
    return_code = 3;
  else {
    long_decimal lvalue_1 = {{0}};
    long_decimal lvalue_2 = {{0}};
    long_decimal longres = {{0}};
    long_decimal mod = {{0}};
    get_equal_scale_long_decimal(value_1, value_2, &lvalue_1, &lvalue_2);
    int_division(lvalue_1, lvalue_2, &longres, &mod);
    long_reduce_scale(&mod);
    return_code = from_ldecimal_to_decimal(mod, result);
  }
  return return_code;
}
