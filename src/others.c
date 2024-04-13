#include "decimal.h"

int dec_floor(decimal value, decimal *result) {
  int return_code = 0;
  decimal one = {{1, 0, 0, 0}};
  set_bit(&one, 1, 127);
  if (result) {
    truncate(value, result);
    if (is_greater(*result, value)) add(*result, one, result);
  } else {
    return_code = 1;
  }
  return return_code;
}

int dec_round(decimal value, decimal *result) {
  int return_code = 0;
  if (result) {
    decimal temp = {{0}};
    decimal one = {{1, 0, 0, 0}};
    float difference;
    int sign = get_bit(value, 127);
    if (sign) set_bit(&value, 0, 127);
    truncate(value, result);
    sub(value, *result, &temp);
    from_decimal_to_float(temp, &difference);
    if (difference >= 0.5) add(*result, one, result);
    set_bit(result, sign, 127);
  } else {
    return_code = 1;
  }
  return return_code;
}

int truncate(decimal value, decimal *result) {
  int return_code = 0;
  if (result) {
    copy_bits(value, result);
    reduce_scale(result, 0);
  } else {
    return_code = 1;
  }
  return return_code;
}

int negate(decimal value, decimal *result) {
  int return_code = 0;
  if (result) {
    copy_bits(value, result);
    int sign = get_bit(value, 127);
    set_bit(result, !sign, 127);
  } else {
    return_code = 1;
  }
  return return_code;
}
