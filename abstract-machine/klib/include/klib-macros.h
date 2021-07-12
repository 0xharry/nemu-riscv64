#ifndef KLIB_MACROS_H__
#define KLIB_MACROS_H__

#define ROUNDUP(a, sz)      ((((uintptr_t)a) + (sz) - 1) & ~((sz) - 1))
#define ROUNDDOWN(a, sz)    ((((uintptr_t)a)) & ~((sz) - 1))
#define LENGTH(arr)         (sizeof(arr) / sizeof((arr)[0]))
#define RANGE(st, ed)       (Area) { .start = (void *)(st), .end = (void *)(ed) }
#define IN_RANGE(ptr, area) ((area).start <= (ptr) && (ptr) < (area).end)

#define STRINGIFY(s)        #s
#define TOSTRING(s)         STRINGIFY(s)
#define _CONCAT(x, y)       x ## y
#define CONCAT(x, y)        _CONCAT(x, y)

#define putstr(s) \
  ({ for (const char *p = s; *p; p++) putch(*p); })


#define OSTREAM_SIZE 1024
#define SIZE_NUM_BUF sizeof(long int)*8
enum BASE {HEX=16, DEC=10};

/* ------------------------ helper of vsprintf -------------------------*/
#define print_unsigned(base) \
  word_count+=print_num(out, va_arg(p_fmt, unsigned long), base, false);

#define print_signed(base) \
  number = va_arg(p_fmt, long); if(number<0) {number = -number;}\
  word_count+=print_num(out, number, base, true);

#define print_pointer()\
  word_count += 2; *out++='0'; *out++='x';\
  print_signed(HEX);

#define print_string();\
  word_count += sputstr(out, va_arg(p_fmt, char*));

#define print_char();\
  *out++=va_arg(p_fmt, int); ++word_count;
/* ------------------------ helper of vsprintf -------------------------*/


// /* ------------------------ helper of vsnprintf -------------------------*/
// #define update_word_count()                                         \
//   if(++word_count>=n) return word_count;

// #define nprint_unsigned(base)                                       \
//   word_count+=nprint_num(out, va_arg(p_fmt, unsigned long), base, false, n-word_count);\
//   if(word_count>=n) return word_count;

// #define nprint_signed(base)                                         \
//   number = va_arg(p_fmt, long);                                     \
//   if(number<0) { number = -number; neg=true;}                       \
//   else         { neg=false; }                                       \
//   word_count+=nprint_num(out, number, base, true, n-word_count);    \
//   if(word_count>=n) return word_count;

// #define nprint_pointer()                                            \
//   *out++='0'; update_word_count();                                  \
//   *out++='x'; update_word_count();                                  \
//   nprint_signed(HEX);

// #define nprint_string();                                            \
//   str = va_arg(p_fmt, char*); number = strlen(str);                 \
//   if(word_count + number > n) number += word_count - n;             \
//   snputstr(out, str, number); return n;

// #define nprint_char();                                              \
//   *out++=va_arg(p_fmt, int); update_word_count();
// /* ------------------------ helper of vsnprintf -------------------------*/


#ifndef _KLIB_TEST_
#define io_read(reg) \
  ({ reg##_T __io_param; \
    ioe_read(reg, &__io_param); \
    __io_param; })

#define io_write(reg, ...) \
  ({ reg##_T __io_param = (reg##_T) { __VA_ARGS__ }; \
    ioe_write(reg, &__io_param); })

#define static_assert(const_cond) \
  static char CONCAT(_static_assert_, __LINE__) [(const_cond) ? 1 : -1] __attribute__((unused))

#define panic_on(cond, s) \
  ({ if (cond) { \
      putstr("AM Panic: "); putstr(s); \
      putstr(" @ " __FILE__ ":" TOSTRING(__LINE__) "  \n"); \
      halt(1); \
    } })

#define panic(s) panic_on(1, s)
#endif
#endif
