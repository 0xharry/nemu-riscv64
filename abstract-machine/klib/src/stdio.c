#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// formatted output conversion

/* convert decimal integer to string */
char* itos_dec(int num, char* str) {
  if(str == NULL) {
    // str = (char*) malloc(12); // 最长 int_min 10位十进制整数+负号+终止符号=12位
    // if(str == NULL)
      assert(0);
  }
  int i = 0;
  int neg = 1;
  if(num<0){
    str[i++] = '-';
    neg = -1;
  }

  int r;
  do {
    r = num%10;
    num /= 10;
    str[i++] = '0'+neg*r;
  } while (num);
  str[i] = '\0';

  char temp;
  char* p;
  if(str[0] == '-') {
    p = str+1;
    i--;
  }
  else {
    p = str;
  }
  for(int j=0; j<i/2; ++j) {
    temp = p[j];
    p[j] = p[i-j-1];
    p[i-j-1] = temp;
  }
  return str;
}

char* itos_hex(int num, char* str) {
  // if(str == NULL) {
  //   // str = (char*) malloc(12); // 最长 int_min 10位十进制整数+负号+终止符号=12位
  //   // if(str == NULL)
  //     assert(0);
  // }
  // int i = 0;

  // int r;
  // do {
  //   r = num%16;
  //   num /= 16;
  //   if(r>9) {
  //     str[i++] = 'A'+r-10;
  //   }
  //   else {
  //     str[i++] = '0'+r;
  //   }
  // } while (num);
  // str[i] = '\0';

  // char temp;
  // char* p = str;
  // for(int j=0; j<i/2; ++j) {
  //   temp = p[j];
  //   p[j] = p[i-j-1];
  //   p[i-j-1] = temp;
  // }
  *str = '1';
  *(str+1) = '\0';
  return str;
}

int vprintf(const char *fmt, va_list p_fmt) {
  int ret_wordcount=0;
  char* s = NULL;
  char num[12];
  int str_len;
  int d;
  while(*fmt != '\0') {
    if(*fmt == '%') {
      switch (*(++fmt)) {
        case 'c':
          d = va_arg(p_fmt, int);
          putch((char)d);
          ++ret_wordcount;
          break;

        case 's':
          s = va_arg(p_fmt, char*);
          str_len = strlen(s);
          putstr(s);
          ret_wordcount += str_len;
          break;

        case 'd':
          d=va_arg(p_fmt, int);
          str_len = strlen(itos_dec(d, num));
          putstr(num);
          ret_wordcount += str_len;
          break;

        case 'p':
          d=va_arg(p_fmt, int);
          str_len = strlen(itos_dec(d, num));
          putstr("0x");
          putstr(num);
          ret_wordcount += str_len;
          break;

        default: break;
      }
      fmt++;
    }
    else {
      putch(*fmt++);
      ++ret_wordcount;
    }
  }
  putch('\0');
  return ret_wordcount;
}

int printf(const char *fmt, ...) {
  // putch(char ch);
  va_list p_fmt; 
  va_start(p_fmt, fmt);
  int ret = vprintf(fmt, p_fmt);
  va_end(p_fmt);
  return ret;
}

/* 
 * The  functions  vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),   vs‐
 * nprintf()   are   equivalent  to  the  functions  printf(),  fprintf(),
 * dprintf(), sprintf(), snprintf(), respectively, except  that  they  are
 * called with a va_list instead of a variable number of arguments.  These
 * functions do not call the va_end macro.  Because they invoke the va_arg
 * macro, the value of ap is undefined after the call.
 */
int vsprintf(char *out, const char *fmt, va_list p_fmt) {
  if(out == NULL) return 0;
  int ret_wordcount=0;
  char* s = NULL;
  int str_len;
  int d;
  while(*fmt != '\0') {
    if(*fmt == '%') {
      switch (*(++fmt)) {
        case 'c':
          d = va_arg(p_fmt, int);
          memcpy(out, (char*)&d, 1);
          ++out;
          ++ret_wordcount;
          break;

        case 's':
          s = va_arg(p_fmt, char*);
          str_len = strlen(s);
          memcpy(out, s, str_len);
          out += str_len;
          ret_wordcount += str_len;
          break;

        case 'd':
          d=va_arg(p_fmt, int);
          str_len = strlen(itos_dec(d, out));
          out += str_len;
          ret_wordcount += str_len;
          break;

        default: break;
      }
      fmt++;
    }
    else {
      *out++ = *fmt++;
      ++ret_wordcount;
    }
  }
  *out = '\0';
  return ret_wordcount;
}

// sprintf()  write output  to  the  given  output  stream
int sprintf(char *out, const char *fmt, ...) {
  va_list p_fmt; 
  va_start(p_fmt, fmt);
  int ret = vsprintf(out, fmt, p_fmt);
  va_end(p_fmt);
  return ret;
}

// int tail()

int vsnprintf(char *out, size_t n, const char *fmt, va_list p_fmt) {
  if(out == NULL || n-- <=0) return 0;
  int ret_wordcount=0;
  char* s = NULL;
  int str_len;
  int d;
  char d_buf[12];
  while(*fmt != '\0') {
    if(*fmt == '%') {
      switch (*(++fmt)) {
        case 'c':
          d = va_arg(p_fmt, int);
          memcpy(out, (char*)&d, 1);
          ++out;
          ++ret_wordcount;
          break;

        case 's':
          s = va_arg(p_fmt, char*);
          str_len = strlen(s);
          if(ret_wordcount + str_len > n) {
            d = n - ret_wordcount;
            memcpy(out, s, d);
            *(out+d+1) = '\0';
            return n;
          }
          memcpy(out, s, str_len);
          out += str_len;
          ret_wordcount += str_len;
          break;

        case 'd':
          d=va_arg(p_fmt, int);
          str_len = strlen(itos_dec(d, d_buf));
          if(ret_wordcount + str_len > n) {
            d = n - ret_wordcount;
            memcpy(out, d_buf, d);
            *(out+d+1) = '\0';
            return n;
          }
          memcpy(out, d_buf, str_len);
          out += str_len;
          ret_wordcount += str_len;
          break;

        default: break;
      }
      fmt++;
    }
    else {
      *out++ = *fmt++;
      ++ret_wordcount;
    }
    if(ret_wordcount >= n) {
      *out = '\0';
      return ret_wordcount;
    }
  }
  *out = '\0';
  return ret_wordcount;
}

/* snprintf
 * The  functions snprintf() and vsnprintf() write at most n bytes (in‐
 * cluding the terminating null byte ('\0')) to out.
 */
int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list p_fmt; 
  va_start(p_fmt, fmt);
  int ret = vsnprintf(out, n, fmt, p_fmt);
  va_end(p_fmt);
  return ret;
}

#endif
