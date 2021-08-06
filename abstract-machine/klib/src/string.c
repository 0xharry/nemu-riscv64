#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *eos = s;
  while (*eos++);
  return (eos - s - 1);
}

char *strcpy(char *dst, const char *src) {
  char *addr = dst;
  while ((*dst++ = *src++) != '\0');
  return addr;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *addr = dst;
  while(n-- && (*dst++ = *src++));
  while(n--) *dst++=0;
  return addr;
}

char *strcat(char *dst, const char *src) {
 
  char *addr = dst;
  //注意这里如果是while(*dst++);当*dst==0时还会++一次,所以修改写法
  while (*dst) dst++; 
  while (*src){
    *dst = *src;
    dst++; src++;
  }
  *dst = '\0';
  return addr;
}

int strcmp(const char *s1, const char *s2) { 
  
  while (*s1)
  {
    if (*s1 == *s2)
    {
      s1++, s2++;
    }
    else
    {
      return *s1 - *s2;
    }
  }
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) { 
  assert(n>=0);

  while(n-- && *s1 && *s1++==*s2++);
  if(n==0) return 0; 

  return *s1-*s2;
}

//这里考虑对齐和高性能有bug，采用简单写法
void *memset(void *v, int c, size_t n) { 
  //align to k 
  //char *dst = (char *)v;
  // size_t k = sizeof(int);
  // unsigned char * dst = (unsigned char *)v;
  // size_t cnt = 0;
  // while(cnt<n){
  //   if((size_t)dst%k==0) break;
  //   *dst++ = (unsigned char)c;
  //   cnt++;
  // }

  // //set k chars one time
  // size_t rest = n-cnt;
  // size_t loop = rest/k;
  // size_t tail = rest%k;

  // int *sint = (int*)dst;
  // for(size_t i=0;i<loop;i++) *sint++ = c;

  // //pad the tail
  // dst = (unsigned char*)sint;
  // for(size_t i=0;i<tail;i++)
  //   *dst++ = (unsigned char)c;
  // return v;

  char *dst = (char *)v;
  while(n--){
    *dst++ = c;
  }
  return v;
}


void *memmove(void *dst, const void *src, size_t n) { 
  char *d = (char *)dst;
  char *s = (char *)src;
  //无重叠
  if ((dst >= src && src + n <= dst) || (dst <= src && dst + n <= src))
  {
    while (n--)
      *d++ = *s++;
  }
  //重叠部分，第一个可以合并
  else if (dst < src && dst + n > src)
  {
    while (n--)
      *d++ = *s++;
  }
  else if (dst > src && src + n > dst)
  {
    char *dend = (char *)(dst + n - 1);
    char *send = (char *)(src + n - 1);
    while (n--)
      *dend-- = *send--;
  }

  return dst; 
}

void *memcpy(void *out, const void *in, size_t n) { 
  return memmove(out, in, n);
}

int memcmp(const void *s1, const void *s2, size_t n) { 
 
  char *p1 = (char *)s1, *p2 = (char *)s2; 
  while (n-->0)
    {
      if (*p1 != *p2)
        return *p1 - *p2;
      p1++, p2++;
    }
  
  return 0;
}

#endif