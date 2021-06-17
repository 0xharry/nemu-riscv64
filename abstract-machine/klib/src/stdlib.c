#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

extern void *brk;
extern const uintptr_t kHEAP_END;
void *malloc(size_t size) {
  // page aligned;
  size = (size + 7) & ~7;
  brk += size;
  // overflow may happen
  if((uintptr_t)brk > kHEAP_END) return NULL;
  return brk;
}

void free(void *ptr) {
}

#endif
