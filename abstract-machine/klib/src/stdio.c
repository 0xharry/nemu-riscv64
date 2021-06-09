#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// formatted output conversion


int printf(const char *fmt, ...) {
  return 0;
}

/* 
 * The  functions  vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),   vs‐
 * nprintf()   are   equivalent  to  the  functions  printf(),  fprintf(),
 * dprintf(), sprintf(), snprintf(), respectively, except  that  they  are
 * called with a va_list instead of a variable number of arguments.  These
 * functions do not call the va_end macro.  Because they invoke the va_arg
 * macro, the value of ap is undefined after the call.
 */
int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

char* itos_dec(int num, char* str) {
  if(str == NULL) {
    // str = (char*) malloc(12); // 最长 int_min 10位十进制整数+负号+终止符号=12位
    // if(str == NULL)
      assert(0);
  }
  int i=0;
  if(num<0){
    str[i++] = '-';
	num = -num;
  }

  int r;
  do {
    r = num%10;
    num /= 10;
    str[i++] = '0'+r;
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
// sprintf()  write output  to  the  given  output  stream
int sprintf(char *out, const char *fmt, ...) {
  int ret_wordcount=0;
  va_list p_fmt; 
  char* s = NULL;
  int str_len;
  int d;
  va_start(p_fmt, fmt);
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
  va_end(p_fmt);
  *out = '\0';
  return ret_wordcount;
}

/* snprintf
 * The  functions snprintf() and vsnprintf() write at most n bytes (in‐
 * cluding the terminating null byte ('\0')) to out.
 */
int snprintf(char *out, size_t n, const char *fmt, ...) {
  if(out == NULL || fmt == NULL || n <=0) return 0;
  int ret_wordcount=0;
  va_list p_fmt; 
  char* s = NULL;
  int str_len;
  int d;
  char d_buf[12];
  va_start(p_fmt, fmt);
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
    if(ret_wordcount >= n)
      return ret_wordcount;
  }
  va_end(p_fmt);
  *out = '\0';
  return ret_wordcount;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
