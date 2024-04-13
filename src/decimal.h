#ifndef SRC_DECIMAL_H_
#define SRC_DECIMAL_H_
#define MY_INF 1.0 / 0.0
#define MY_NAN 0.0 / 0.0

enum { low, mid, top, scale };

typedef struct decimal {
  unsigned int bits[4];
} decimal;

enum { lowest, lower, llow, lmid, ltop, higher, highest, lscale };

typedef struct long_decimal {
  unsigned int bits[8];
} long_decimal;

// arithmetic
int add(decimal value_1, decimal value_2, decimal *result);
int sub(decimal value_1, decimal value_2, decimal *result);
int mul(decimal value_1, decimal value_2, decimal *result);
int div(decimal value_1, decimal value_2, decimal *result);
int mod(decimal value_1, decimal value_2, decimal *result);
// comparisons
int is_less(decimal value_1, decimal value_2);
int is_less_or_equal(decimal value_1, decimal value_2);
int is_greater(decimal value_1, decimal value_2);
int is_greater_or_equal(decimal value_1, decimal value_2);
int is_equal(decimal value_1, decimal value_2);
int is_not_equal(decimal value_1, decimal value_2);
// converters
int from_int_to_decimal(int src, decimal *dst);
int from_float_to_decimal(float src, decimal *dst);
int from_decimal_to_int(decimal src, int *dst);
int from_decimal_to_float(decimal src, float *dst);
// others
int dec_floor(decimal value, decimal *result);
int dec_round(decimal value, decimal *result);
int truncate(decimal value, decimal *result);
int negate(decimal value, decimal *result);
// utils
void set_scale(decimal *dst, int exp);
int get_scale(decimal *src);
void set_bit(decimal *val, int bit, int pos);
int get_bit(decimal val, int pos);
void clear_bits(decimal *dec);
long double pow_two(int scale);
int get_positive_float(float *num);
void reduce_scale(decimal *value, int req_scale);
void set_scale_mask(decimal *value);
int is_bits_zero(decimal value);
void copy_bits(decimal src, decimal *dst);
// long_utils
void long_set_scale(long_decimal *dst, int exp);
int long_get_scale(long_decimal *src);
int long_get_bit(long_decimal val, int pos);
void long_set_bit(long_decimal *val, int bit, int pos);
void long_clear_bits(long_decimal *dec);
void long_copy_bits(long_decimal src, long_decimal *dst);
int long_is_bits_zero(long_decimal lvalue);
int long_is_bits_equal(long_decimal value_1, long_decimal value_2);
int long_is_bits_less(long_decimal value_1, long_decimal value_2);
int long_is_bits_greater(long_decimal value_1, long_decimal value_2);
int addition(long_decimal value_1, long_decimal value_2, long_decimal *result);
int subtraction(long_decimal value_1, long_decimal value_2,
                long_decimal *result);
void multiplication(long_decimal value_1, long_decimal value_2,
                    long_decimal *result);
void int_division(long_decimal lvalue_1, long_decimal lvalue_2,
                  long_decimal *longres, long_decimal *mod);
void mod_division(long_decimal lvalue_1, long_decimal lvalue_2,
                  decimal *result);
void long_shift_left(long_decimal *lvalue);
void long_shift_right(long_decimal *lvalue);
void from_decimal_to_ldecimal(decimal src, long_decimal *dst);
int from_ldecimal_to_decimal(long_decimal src, decimal *dst);
void long_increase_scale(long_decimal *lvalue, int req_scale);
void long_reduce_scale(long_decimal *lvalue);
void get_equal_scale_long_decimal(decimal value_1, decimal value_2,
                                  long_decimal *lvalue_1,
                                  long_decimal *lvalue_2);
#endif  // SRC_DECIMAL_H_
