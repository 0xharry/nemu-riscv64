#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  return 0;
}

/* strcpy, strncpy - copy a string
 * The  strcpy() function copies the string pointed to by src, including the termi‐
 * nating null byte ('\0'), to the buffer pointed to by dest.  The strings may  not
 * overlap,  and  the  destination  string dest must be large enough to receive the
 * copy.
 */
char *strcpy(char* dst,const char* src) {
  if(dst != NULL && src != NULL) do{
    *dst = *src;
  } while(src!='\0');
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

/* strcat - concatenate two strings
 * the strcat() function appends the src string to the dest string, overwriting the  
 * terminating null byte ('\\0') at the end of dest, and  then  adds  a  terminating  
 * null  byte.
 * The strings may not overlap, and the dest string must have enough space  for  the 
 * result.  If dest is not large enough, program behavior  is  unpredictable;
 */
char* strcat(char* dst, const char* src) {
  char* end_of_dst=dst;
  char* p_src = src;
  for(; *end_of_dst != '\0'; ++end_of_dst);
  for(; *p_src != '\0'; ++end_of_dst, ++p_src) {
    *end_of_dst = *p_src;
  }
  *(end_of_dst+1) = '\0';
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
    ret = *s1-*s2;
    if(ret != 0)
      return ret;
  }
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

/* 
 * memset - fill memory with a constant byte
 * The  memset()  function fills the first n bytes of the memory area pointed to
 * by s with the constant byte c.
 */
void* memset(void* v,int c,size_t n) {
  for(int i=0; i<n; ++i) {
    *(char *)v = (char) c;
  }
  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  return 0;
}

#endif
