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

#ifdef _KLIB_TEST_
#define putstr(s) \
  ({ for (const char *p = s; *p; p++) putchar(*p); })
#endif

#define OSTREAM_SIZE 1024
#define SIZE_NUM_BUF sizeof(long int)*8
enum BASE {HEX=16, DEC=10};

/* ------------------------ helper of vsprintf -------------------------*/
#define print_unsigned(base) \
  temp = print_num(out, va_arg(p_fmt, unsigned long), base, false);\
  word_count+=temp; out += temp;

#define print_signed(base) \
  number = va_arg(p_fmt, int);\
  if(number<0)  { number = -number; negtive_sign = true; }\
  else          { negtive_sign = false; }\
  temp = print_num(out, number, base, negtive_sign);\
  word_count+=temp; out += temp;

#define print_pointer()\
  word_count += 2; *out++='0'; *out++='x';\
  temp = print_num(out, (unsigned long)va_arg(p_fmt, void*), HEX, false);\
  word_count+=temp; out += temp;

#define print_string();\
  temp = sputstr(out, va_arg(p_fmt, char*));\
  word_count+=temp; out += temp;

#define print_char();\
  *out++=va_arg(p_fmt, int); ++word_count;
/* ------------------------ helper of vsprintf -------------------------*/


#ifndef _KLIB_TEST_
#define putstr(s) \
  ({ for (const char *p = s; *p; p++) putch(*p); })

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
