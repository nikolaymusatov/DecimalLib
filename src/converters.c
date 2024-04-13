#include "decimal.h"

int from_int_to_decimal(int src, decimal *dst) {
  int return_code = 0;
  if (dst) {
    clear_bits(dst);
    if (src < 0) {
      set_bit(dst, 1, 127);
      src = -src;
    }
    dst->bits[low] = src;
  } else
    return_code = 1;
  return return_code;
}

int from_float_to_decimal(float src, decimal *dst) {
  clear_bits(dst);
  int return_code = 0;
  int scale = 0;
  int sign = get_positive_float(&src);
  unsigned src_bits = *((unsigned *)&src);
  int exp = (src_bits >> 23) - 127;
  for (; src && !(int)(src / 10000000); scale++) src *= 10;
  if (scale > 28 || exp > 95 || !dst)
    return_code = 1;
  else {
    src_bits = *((unsigned *)&src);
    exp = (src_bits >> 23) - 127;
    set_bit(dst, 1, exp);
    for (unsigned mask = 0x400000; mask; mask >>= 1, exp--)
      set_bit(dst, !!(src_bits & mask), exp - 1);
    set_scale(dst, scale);
    if (sign) set_bit(dst, 1, 127);
  }
  return return_code;
}

int from_decimal_to_int(decimal src, int *dst) {
  int return_code = 0;
  int sign = 0;
  if (dst) {
    if (get_bit(src, 127)) sign = 1;
    decimal res = {{0}};
    truncate(src, &res);
    if (res.bits[top] || res.bits[mid])
      return_code = 1;
    else
      *dst = res.bits[low];
    if (sign) *dst *= -1;
  } else
    return_code = 1;
  return return_code;
}

int from_decimal_to_float(decimal src, float *dst) {
  int return_code = 0;
  long double res = 0.0;
  int sign = get_bit(src, 127);
  if (dst) {
    *dst = 0;
    for (int i = 0; i < 96; i++) res += get_bit(src, i) * pow_two(i);
    int scale = get_scale(&src);
    for (int i = 0; i < scale; i++) res /= 10;
    if (sign) res *= -1;
    *dst = (float)res;
  } else
    return_code = 1;
  return return_code;
}
