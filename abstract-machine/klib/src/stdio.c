#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// formatted output conversion


int printf(const char *fmt, ...) {
  return 0;
}

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
  va_list p_fmt; 
  char* s = NULL;
  int str_len;
  int d;
  char temp;
  va_start(p_fmt, fmt);
  while(*fmt != '\0') {
    if(*fmt == '%') {
      switch (*(++fmt)) {
        case 'c':
          s = va_arg(p_fmt, char);
          memcpy(out, s, 1);
          ++out;
          break;

        case 's':
          s = va_arg(p_fmt, char *);
          str_len = strlen(s);
          memcpy(out, s, str_len);
          out += str_len;
          break;

        case 'd':
          d=va_arg(p_fmt, int);
          str_len = strlen(itos_dec(d, out));
          out += str_len;
          break;

        default: break;
      }
    }
    else {
      *out++ = *fmt++;
    }
    fmt++;
  }
  va_end(p_fmt);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
