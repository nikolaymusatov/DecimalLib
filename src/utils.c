#include "decimal.h"

void set_scale(decimal *dst, int exp) {
  int sign = 0;
  if (dst->bits[scale] >> 31 == 1) sign = 1;
  dst->bits[scale] = exp << 16;
  set_bit(dst, sign, 127);
}

int get_scale(decimal *src) {
  int result = src->bits[scale] >> 16;
  if ((src->bits[scale] >> 31) & 1) result &= ~32768;
  return result;
}

void set_bit(decimal *val, int bit, int pos) {
  if (bit)
    val->bits[pos / 32] = val->bits[pos / 32] | (bit << pos % 32);
  else
    val->bits[pos / 32] = val->bits[pos / 32] & ~((bit + 1) << pos % 32);
}

int get_bit(decimal val, int pos) {
  int bit = (val.bits[pos / 32] >> (pos % 32)) & 1;
  return bit;
}

void clear_bits(decimal *dec) {
  for (int i = 0; i < 4; i++) dec->bits[i] = 0;
}

long double pow_two(int scale) {
  long double res = 1.0;
  for (int i = 0; i < scale; i++) res *= 2;
  return res;
}

int get_positive_float(float *num) {
  int sign = 0;
  if (*num < 0) {
    sign = 1;
    *num *= -1;
  }
  return sign;
}

int is_bits_zero(decimal value) {
  int return_code = 0;
  if (value.bits[low] == 0 && value.bits[mid] == 0 && value.bits[top] == 0)
    return_code = 1;
  return return_code;
}

void copy_bits(decimal src, decimal *dst) {
  dst->bits[low] = src.bits[low];
  dst->bits[mid] = src.bits[mid];
  dst->bits[top] = src.bits[top];
  dst->bits[scale] = src.bits[scale];
}

void reduce_scale(decimal *value, int req_scale) {
  int scale = get_scale(value);
  for (; scale > req_scale; scale--) {
    unsigned long long buf = 0ull;
    unsigned long long div_result = 0ull;
    unsigned mod_digit = 0;
    for (int i = 2; i >= 0; i--) {
      mod_digit = (buf + (unsigned long long)value->bits[i]) % 10ull;
      div_result = (buf + (unsigned long long)value->bits[i]) / 10ull;
      value->bits[i] = (unsigned)div_result;
      buf = mod_digit;
      buf <<= 32;
    }
  }
  set_scale(value, scale);
}

void set_scale_mask(decimal *value) {
  value->bits[scale] &= 0b10000000111111110000000000000000;
}
