#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len=0;
  for(;s[len]!='\0'; ++len);
  return len;
}

/* strcpy, strncpy - copy a string
 * The  strcpy() function copies the string pointed to by src, including the termi‐
 * nating null byte ('\0'), to the buffer pointed to by dest.  The strings may  not
 * overlap,  and  the  destination  string dest must be large enough to receive the
 * copy.
 */
char *strcpy(char* dst,const char* src) {
  if(dst == NULL || src == NULL) return dst;
  char* p_dst = dst;
  const char* p_src = src;
  while((bool)(*p_dst++ = *p_src++));
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  if(dst == NULL || src == NULL || n <= 0) return dst;
  char* p_dst = dst;
  const char* p_src = src;
  while((bool)(*p_dst++ = *p_src++) && n--);
  return dst;
}

/* strcat - concatenate two strings
 * the strcat() function appends the src string to the dest string, overwriting the  
 * terminating null byte ('\\0') at the end of dest, and  then  adds  a  terminating  
 * null  byte.
 * The strings may not overlap, and the dest string must have enough space  for  the 
 * result.  If dest is not large enough, program behavior  is  unpredictable;
 */
char* strcat(char* dst, const char* src) {
  char* end_of_dst = dst;
  const char* p_src = src;
  for(;*end_of_dst != '\0'; ++end_of_dst);
  while((bool)(*end_of_dst++ = *p_src++));
  return dst;
}


/* strcmp - compare two strings
 * The  strcmp()  function  compares  the two strings s1 and s2.  The locale is not
 * taken into account (for a locale-aware comparison, see strcoll(3)).  It  returns
 * an  integer  less  than,  equal to, or greater than zero if s1 is found, respec‐
 * tively, to be less than, to match, or be greater than s2.
 */
int strcmp(const char* s1, const char* s2) {
  int ret = 0;
  while(*s1!='\0' || *s2!='\0') {
    ret = *s1++-*s2++;
    if(ret != 0)
      return ret;
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  int ret = 0;
  while((*s1!='\0' || *s2!='\0') && n--) {
    ret = *s1++-*s2++;
    if(ret != 0)
      return ret;
  }
  return 0;
}

/* 
 * memset - fill memory with a constant byte
 * The  memset()  function fills the first n bytes of the memory area pointed to
 * by s with the constant byte c.
 */
void* memset(void* v,int c,size_t n) {
  char* p_v = (char*)v;
  while(n--) {
    *p_v++ = (char) c;
  }
  return v;
}
/* copy from Duan */
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
// /* 
//  * memcpy - copy memory area
//  * The  memcpy()  function  copies  n bytes from memory area src to memory
//  * area dest.  The memory areas must not overlap.  Use memmove(3)  if  the
//  * memory areas do overlap.
//  */
// void* memcpy(void* restrict_dst, const void* restrict_src, size_t n) {
//   char* p_out = restrict_dst;
//   while(n--){
//     *p_out++ = *(char*)restrict_src++;
//   }
//   return restrict_dst;
// }

// /* memmove - copy memory area
//  * The  memmove()  function  copies n bytes from memory area src to memory
//  * area dest.  The memory areas may overlap: copying takes place as though
//  * the  bytes in src are first copied into a temporary array that does not
//  * overlap src or dest, and the bytes are then copied from  the  temporary
//  * array to dest.
//  */
// void* memmove(void* dst,const void* src,size_t n) {
//   if((dst+n-src)*(dst-src+n)>0) {
//     memcpy(dst, src, n);
//     return dst;
//   }
//   char temp[n];
//   memcpy(temp, src, n);
//   memcpy(dst, temp, n);
//   return dst;
// }

/* 
 * memcmp - compare memory areas
 * The  memcmp()  function compares the first n bytes (each interpreted as
 * unsigned char) of the memory areas s1 and s2.
 */
int memcmp(const void* s1, const void* s2, size_t n) {
  int ret = 0;
  while(n--) {
    ret = *(char*)s1++-*(char*)s2++;
    if(ret != 0)
      return ret;
  }
  return 0;
}

#endif
