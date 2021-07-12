
#ifdef _KLIB_TEST_
#include "../tests/klib_test.h"
#else
#include <am.h>
#include <klib.h>
#endif

#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// formatted output conversion

/*------------------ function helper ----------------------------*/
static inline bool isdigit(char c) {
  return ((c >= '0') && (c <= '9'));
}

static inline int sputstr(char *out, const char *s) {
  int i=0;
  for (; s[i] != '\0'; ++i) out[i]=s[i];
  return i;
}

static inline int snputstr(char *out, const char *s, size_t n) {
  int i=0;
  for (; (s[i] != '\0') && (n--); ++i) out[i]=s[i];
  return i;
}

static char num_buf[SIZE_NUM_BUF];
static char num2char[] = "0123456789abcdef";
static int field_width;
static bool zero_padding;
static int print_num(char *out, unsigned long val, unsigned base, bool neg_sign) {
  int word_count=0;
  int length=0;
  char pad_with=0;
  // convert number from 'int' to 'str'(reversed)
  do {
    num_buf[length++] = num2char[val%base];
    val /= base;
  }while (val);

  // print length with padding
  if(field_width > length) {
    if(zero_padding)
      pad_with = '0';
    else
      pad_with = ' ';
    
    int pad_len = field_width-length;
    word_count += pad_len;
    while(--pad_len)
      *out++ = pad_with;

    if(neg_sign)
      *out++ = '-';
    else
      *out++ = pad_with;
  }

  // print num_buf to 'out'
  word_count += length;
  do {
    *out++ = num_buf[--length];
  }while (length);

  return word_count;
}
/*------------------ function helper ----------------------------*/


/* 
 * The  functions  vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),   vs‐
 * nprintf()   are   equivalent  to  the  functions  printf(),  fprintf(),
 * dprintf(), sprintf(), snprintf(), respectively, except  that  they  are
 * called with a va_list instead of a variable number of arguments.  These
 * functions do not call the va_end macro.  Because they invoke the va_arg
 * macro, the value of ap is undefined after the call.
 * 
 *  This version implements the following printf features:
 *	%d	decimal conversion
 *	%u	unsigned conversion
 *	%x	hexadecimal conversion
 *  %p  address start with '0x'
 *	%c	character
 *	%s	string
 *	%m	field width
 *	%0m	zero-padding
 */

int vsprintf(char *out, const char *fmt, va_list p_fmt) {
  // if(out == NULL) return 0;
  int word_count=0;
  long number=0;
  field_width=0;
  zero_padding=0;
  char fmt_c=0;
  while (*fmt != '\0') {
    while (*fmt != '%') {
      *out++ = *fmt++;
      ++word_count;
    }
    ++fmt;

    fmt_c = *fmt++;
    // zero-padding
    if(fmt_c == '0') {
      zero_padding = 1;
      fmt_c = *fmt++;
    }

    // field width
    if(isdigit(fmt_c))
      while (isdigit(fmt_c)) {
        field_width = 10*field_width + fmt_c - '0';
        fmt_c = *fmt++;
      }

    // print fomarts
    switch (*fmt++) {
    case 'u': print_unsigned(DEC); break;
    case 'd': print_signed(DEC);   break;
    case 'x': print_signed(HEX);   break;
    case 'p': print_pointer();     break;
    case 's': print_string();      break;
    case 'c': print_char();        break;
    default:                       break;
    }
  }
  
  // print stops
  *out = '\0';
  ++word_count;
  return word_count;
}

// sprintf()  write output  to  the  given  output  stream
int sprintf(char *out, const char *fmt, ...) {
  va_list p_fmt; 
  va_start(p_fmt, fmt);
  int ret = vsprintf(out, fmt, p_fmt);
  va_end(p_fmt);
  return ret;
}

static char ostream[OSTREAM_SIZE];
int printf(const char *fmt, ...) {
  va_list p_fmt; 
  va_start(p_fmt, fmt);
  int ret = vsprintf(ostream, fmt, p_fmt);
  va_end(p_fmt);
  putstr(ostream);
  return ret;
}

/* -------------------------- n printf() -------------------------------*/

/* 
 * The  functions snprintf() and vsnprintf() write at most n bytes (in‐
 * cluding the terminating null byte ('\0')) to out.
 */
int vsnprintf(char *out, size_t n, const char *fmt, va_list p_fmt) {
  // if(out == NULL || n-- <=0 || n>1023) return 0;
  int ret = vsprintf(ostream, fmt, p_fmt);
  memcpy(out, ostream, n);
  return ret>n? n: ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list p_fmt; 
  va_start(p_fmt, fmt);
  int ret = vsprintf(ostream, fmt, p_fmt);
  va_end(p_fmt);
  memcpy(out, ostream, n);
  return ret>n? n: ret;
}

#endif
