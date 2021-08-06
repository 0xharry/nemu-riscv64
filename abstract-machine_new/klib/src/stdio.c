#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define is_digit(c) ((c)>='0' &&(c)<='9')





/**
 * @brief 转换int 到 string
 * 
 * @param n 
 * @param radix 
 * @param ret 
 * @return int 
 */
int i2a(int n,int base,char *ret) { 
  
  int flag = 0;
  if(n<0 ) flag =1, n=-n;

  char buf[128] = {0};
  int i=0,j=0;
  if(n==0) { 
    *ret = '0';
    return 1;
  }
  while(n>0){
    buf[i++] = n%base + '0';
    n/=base;
  }
 
  if(flag == 1 && base == 10) buf[i++]='-';
  buf[i]='\0';

  for(j=i-1;j>=0;j--){
    if(is_digit(buf[i-j-1]) || buf[i-j-1]=='-')
      ret[j] = buf[i-j-1];
    else
      ret[j] = buf[i-j-1]-'9'+'a'-1;
  }
  ret[i] = '\0';
 
  return i;
}

int printf(const char *fmt, ...) {
  va_list ap;
  char buf[4096] = {0};
  va_start(ap, fmt);
  int len = vsprintf(buf,fmt,ap);
  va_end(ap);
  size_t i = 0;
  while(buf[i] != '\0') {
    putch(buf[i]);
    i++;
  }
  return len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int arg_int;
  char *arg_s;
  int len=0;
  int base=10;
  size_t pos=0;

  while(*fmt){
    switch(*fmt){
    case '%':
      ++fmt;
      switch(*fmt){
        case 'd': 
          base = 10;
          arg_int = va_arg(ap, int);
          len = i2a(arg_int,base,out+pos);
          pos+=len;
          break;
        case 'x':
          base = 16;
          arg_int = va_arg(ap, int);
          len = i2a(arg_int,base,out+pos);
          pos+=len;
          break;
        case 's': 
          // putch('s');
          arg_s = va_arg(ap,char *);
          strcat(out,arg_s);
          len = strlen(arg_s);
          pos+=len;
          break;
        case 'c':
          arg_int = va_arg(ap,int);
          out[pos] = arg_int+'0';
          pos++;
          break;
        case 'p':
          out[pos++] = '0';
          out[pos++] = 'x';
          base = 16;
          arg_int = va_arg(ap, int);
          len = i2a(arg_int,base,out+pos);
          pos+=len;
          break;
        default: break;
      }
      break;
    default: 
      out[pos++]=*fmt;
      break;
  }
  fmt++;
  }
  out[pos] = '\0';
  va_end(ap);
  return pos;
}

int sprintf(char *out, const char *fmt, ...) {
    //memset(buf,0,sizeof(char)*1024);
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
    assert(0);
    return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    assert(0);
    return 0;
}

//TODO:implement sscanf


#endif