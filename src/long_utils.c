#include "decimal.h"

void from_decimal_to_ldecimal(decimal src, long_decimal *dst) {
  dst->bits[lowest] = src.bits[low];
  dst->bits[lower] = src.bits[mid];
  dst->bits[llow] = src.bits[top];
  dst->bits[lmid] = 0;
  dst->bits[ltop] = 0;
  dst->bits[higher] = 0;
  dst->bits[highest] = 0;
  dst->bits[lscale] = src.bits[scale];
}

int from_ldecimal_to_decimal(long_decimal src, decimal *dst) {
  int return_code = 0;
  clear_bits(dst);
  int sign = long_get_bit(src, 255);
  if (long_get_scale(&src) >= 256 || src.bits[lmid])
    return_code = sign + 1;
  else {
    dst->bits[low] = src.bits[lowest];
    dst->bits[mid] = src.bits[lower];
    dst->bits[top] = src.bits[llow];
    dst->bits[scale] = src.bits[lscale];
  }
  return return_code;
}

void long_reduce_scale(long_decimal *lvalue) {
  int scale = long_get_scale(lvalue);
  long_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
  for (; lvalue->bits[lmid] || scale > 28; scale--) {
    unsigned long long buf = 0ull;
    unsigned long long div_result = 0ull;
    unsigned mod_digit = 0;
    for (int i = 6; i >= 0; i--) {
      mod_digit = (buf + (unsigned long long)lvalue->bits[i]) % 10ull;
      div_result = (buf + (unsigned long long)lvalue->bits[i]) / 10ull;
      lvalue->bits[i] = (unsigned)div_result;
      buf = mod_digit;
      buf <<= 32;
    }
    if (mod_digit == 5 && (div_result % 10) % 2 == 1)
      addition(*lvalue, one, lvalue);
    else if (mod_digit > 5)
      addition(*lvalue, one, lvalue);
  }
  long_set_scale(lvalue, scale);
}

void long_increase_scale(long_decimal *lvalue, int req_scale) {
  int scale = long_get_scale(lvalue);
  for (; scale < req_scale; scale++) {
    unsigned long long buf = 0ull;
    for (int i = 0; i <= 6; i++) {
      buf = buf + ((unsigned long long)lvalue->bits[i] * 10ull);
      lvalue->bits[i] = (unsigned)buf;
      buf >>= 32;
    }
  }
  long_set_scale(lvalue, scale);
}

void long_set_scale(long_decimal *dst, int exp) {
  int sign = 0;
  if (dst->bits[lscale] >> 31 == 1) sign = 1;
  dst->bits[lscale] = exp << 16;
  long_set_bit(dst, sign, 255);
}

int long_get_scale(long_decimal *src) {
  int result = src->bits[lscale] >> 16;
  if ((src->bits[lscale] >> 31) & 1) result &= ~32768;
  return result;
}

int long_get_bit(long_decimal val, int pos) {
  int bit = (val.bits[pos / 32] >> (pos % 32)) & 1;
  return bit;
}

void long_set_bit(long_decimal *val, int bit, int pos) {
  if (bit)
    val->bits[pos / 32] = val->bits[pos / 32] | (bit << pos % 32);
  else
    val->bits[pos / 32] = val->bits[pos / 32] & ~((bit + 1) << pos % 32);
}

void long_clear_bits(long_decimal *dec) {
  for (int i = 0; i < 8; i++) dec->bits[i] = 0;
}

void long_copy_bits(long_decimal src, long_decimal *dst) {
  dst->bits[lowest] = src.bits[lowest];
  dst->bits[lower] = src.bits[lower];
  dst->bits[llow] = src.bits[llow];
  dst->bits[lmid] = src.bits[lmid];
  dst->bits[ltop] = src.bits[ltop];
  dst->bits[higher] = src.bits[higher];
  dst->bits[highest] = src.bits[highest];
  dst->bits[lscale] = src.bits[lscale];
}

int long_is_bits_equal(long_decimal value_1, long_decimal value_2) {
  int return_code = 0;
  if (value_1.bits[lowest] == value_2.bits[lowest] &&
      value_1.bits[lower] == value_2.bits[lower] &&
      value_1.bits[llow] == value_2.bits[llow] &&
      value_1.bits[lmid] == value_2.bits[lmid] &&
      value_1.bits[ltop] == value_2.bits[ltop] &&
      value_1.bits[higher] == value_2.bits[higher] &&
      value_1.bits[highest] == value_2.bits[highest] &&
      value_1.bits[lscale] == value_2.bits[lscale])
    return_code = 1;
  return return_code;
}

int long_is_bits_less(long_decimal value_1, long_decimal value_2) {
  int return_code = 0;
  int i = 6;
  int comparing = 1;
  while (return_code == 0 && i >= 0 && comparing) {
    if (value_1.bits[i] < value_2.bits[i])
      return_code = 1;
    else if (value_1.bits[i] > value_2.bits[i])
      comparing = 0;
    else
      i--;
  }
  return return_code;
}

int long_is_bits_greater(long_decimal value_1, long_decimal value_2) {
  int return_code = 0;
  int i = 6;
  int comparing = 1;
  while (return_code == 0 && i >= 0 && comparing) {
    if (value_1.bits[i] > value_2.bits[i])
      return_code = 1;
    else if (value_1.bits[i] < value_2.bits[i])
      comparing = 0;
    else
      i--;
  }
  return return_code;
}

void get_equal_scale_long_decimal(decimal value_1, decimal value_2,
                                  long_decimal *lvalue_1,
                                  long_decimal *lvalue_2) {
  set_scale_mask(&value_1);
  set_scale_mask(&value_2);
  from_decimal_to_ldecimal(value_1, lvalue_1);
  from_decimal_to_ldecimal(value_2, lvalue_2);
  int scale_1 = get_scale(&value_1);
  int scale_2 = get_scale(&value_2);
  if (scale_1 < scale_2) long_increase_scale(lvalue_1, scale_2);
  if (scale_2 < scale_1) long_increase_scale(lvalue_2, scale_1);
}

int addition(long_decimal value_1, long_decimal value_2, long_decimal *result) {
  int return_code = 0;
  int buf = 0;
  int bit_1 = 0;
  int bit_2 = 0;
  for (int i = 0; i < 224; i++) {
    bit_1 = long_get_bit(value_1, i);
    bit_2 = long_get_bit(value_2, i);
    if (bit_1 + bit_2 + buf == 0) {
      long_set_bit(result, 0, i);
      buf = 0;
    } else if (bit_1 + bit_2 + buf == 1) {
      long_set_bit(result, 1, i);
      buf = 0;
    } else if (bit_1 + bit_2 + buf == 2) {
      long_set_bit(result, 0, i);
      buf = 1;
    } else if (bit_1 + bit_2 + buf == 3) {
      long_set_bit(result, 1, i);
      buf = 1;
    }
  }
  long_set_scale(result, long_get_scale(&value_1));
  return return_code;
}

int subtraction(long_decimal value_1, long_decimal value_2,
                long_decimal *result) {
  int return_code = 0;
  int bit_1 = 0;
  int bit_2 = 0;
  for (int i = 223; i >= 0; i--) {
    bit_1 = long_get_bit(value_1, i);
    bit_2 = long_get_bit(value_2, i);
    if (bit_1 - bit_2 == 0) {
      long_set_bit(result, 0, i);
    } else if (bit_1 - bit_2 == 1) {
      long_set_bit(result, 1, i);
    } else if (bit_1 - bit_2 == -1) {
      long_set_bit(result, 1, i);
      for (int b = i + 1; b < 224; b++) {
        if (long_get_bit(*result, b) == 0)
          long_set_bit(result, 1, b);
        else {
          long_set_bit(result, 0, b);
          break;
        }
      }
    }
  }
  long_set_scale(result, long_get_scale(&value_1));
  return return_code;
}

void multiplication(long_decimal value_1, long_decimal value_2,
                    long_decimal *result) {
  long_decimal temp = {{0}};
  long_clear_bits(result);
  for (int i = 0; i < 224; i++) {
    int bit = long_get_bit(value_2, i);
    temp.bits[lowest] = value_1.bits[lowest] * bit;
    temp.bits[lower] = value_1.bits[lower] * bit;
    temp.bits[llow] = value_1.bits[llow] * bit;
    temp.bits[lmid] = value_1.bits[lmid] * bit;
    temp.bits[ltop] = value_1.bits[ltop] * bit;
    temp.bits[higher] = value_1.bits[higher] * bit;
    temp.bits[highest] = value_1.bits[highest] * bit;
    for (int t = 0; t < i; t++) {
      long_shift_left(&temp);
    }
    addition(temp, *result, result);
  }
  long_set_scale(result, long_get_scale(&value_1) * 2);
}

void int_division(long_decimal lvalue_1, long_decimal lvalue_2,
                  long_decimal *longres, long_decimal *mod) {
  int shifts = 0;
  int index = 0;
  long_decimal temp = {{0}};
  int sign = long_get_bit(lvalue_1, 255) != long_get_bit(lvalue_2, 255);
  int modsign = long_get_bit(lvalue_1, 255);
  if (!long_is_bits_zero(lvalue_1) && long_is_bits_less(lvalue_2, lvalue_1)) {
    for (int i = 223; !long_get_bit(lvalue_1, i); i--) index = i - 1;
    while (!long_get_bit(lvalue_2, index)) {
      long_shift_left(&lvalue_2);
      shifts += 1;
    }
  }
  for (int i = 0; i <= shifts; i++) {
    if (!long_is_bits_less(lvalue_1, lvalue_2)) {
      long_shift_left(longres);
      long_set_bit(longres, 1, 0);
      subtraction(lvalue_1, lvalue_2, &temp);
      long_copy_bits(temp, &lvalue_1);
    } else {
      long_shift_left(longres);
    }
    long_shift_left(&lvalue_1);
  }
  long_copy_bits(lvalue_1, mod);
  long_set_bit(longres, sign, 255);
  long_set_bit(mod, modsign, 255);
  for (int i = 0; i <= shifts; i++) long_shift_right(mod);
}

void mod_division(long_decimal lvalue_1, long_decimal lvalue_2,
                  decimal *result) {
  int scale = 0;
  decimal sum = {{0}};
  long_decimal mod = {{0}};
  long_decimal longres = {{0}};
  for (int i = 0; i < 28 && !long_is_bits_zero(lvalue_1); i++) {
    long_increase_scale(&lvalue_1, long_get_scale(&lvalue_1) + 1);
    int_division(lvalue_1, lvalue_2, &longres, &mod);
    long_copy_bits(mod, &lvalue_1);
    long_increase_scale(&longres, long_get_scale(&longres) + 1);
    from_ldecimal_to_decimal(longres, &sum);
    scale = long_get_scale(&longres);
    add(*result, sum, result);
    long_clear_bits(&longres);
    long_set_scale(&longres, scale);
  }
  set_scale(result, long_get_scale(&lvalue_1) + 1);
}

void long_shift_left(long_decimal *lvalue) {
  for (int g = 6; g >= 0; g--) {
    if (g) {
      int bit = long_get_bit(*lvalue, 32 * g - 1);
      lvalue->bits[g] <<= 1;
      long_set_bit(lvalue, bit, 32 * g);
    } else {
      lvalue->bits[g] <<= 1;
    }
  }
}

void long_shift_right(long_decimal *lvalue) {
  for (int g = 0; g <= 6; g++) {
    if (g) {
      int bit = long_get_bit(*lvalue, 32 * g);
      lvalue->bits[g] >>= 1;
      long_set_bit(lvalue, bit, 32 * g - 1);
    } else {
      lvalue->bits[g] >>= 1;
    }
  }
}

int long_is_bits_zero(long_decimal lvalue) {
  int return_code = 0;
  if (lvalue.bits[lowest] == 0 && lvalue.bits[lower] == 0 &&
      lvalue.bits[llow] == 0 && lvalue.bits[lmid] == 0 &&
      lvalue.bits[ltop] == 0 && lvalue.bits[higher] == 0 &&
      lvalue.bits[highest] == 0)
    return_code = 1;
  return return_code;
}
